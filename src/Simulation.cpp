/* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Beikirch, Maximilian
 * Cramer, Simon
 * Frank, Martin
 * Otte, Philipp
 * Pabich, Emma
 * Trimborn, Torsten
 *
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <cassert>
#include <iostream>
#include <chrono>
#include <boost/optional.hpp>

#include "Simulation.h"

#include "Exceptions/MathError.h"
#include "RandomGenerator/RandomNumberPool.h"

#if WITH_OPENMP
#include <omp.h>
#endif

#if WITH_PROGBAR
#include "progressbar.h"
#endif


using namespace boost;
using namespace std;


/** Standard constructor
 */
Simulation::Simulation() {

    stockExchange = nullptr;
    excessDemandCalculator = nullptr;
    dataCollector = nullptr;
    priceCalculator = nullptr;
    simulationTime = 0;
    numSteps = 0;
    variableContainer = nullptr;
    randomNumberPool = nullptr;
    shareCalculator = nullptr;
    quantitiesOfInterest = nullptr;

}


/** Parse the parameter object, create all objects and connect them.
 *	Requires a lot of computation time due to all the if clauses.
 */
void Simulation::parse(Input &input) {


    this->input = input;

    assert(input("numSteps"));

    //RandomGenerator
    randomNumberPool = RandomGenerator::factory(input);


    //Variable Containers
    variableContainer = VariableContainer::factory(input, randomNumberPool);

    //Agents
    agents = Agent::factory(input, randomNumberPool, variableContainer->price,
                            variableContainer->excessDemand, variableContainer->globalNews,
                            variableContainer->dividend, variableContainer->deltaT, switchables);

    /// @todo: only works for one parameter set yet
    /// agents might be switchable ( in order to have a getChartistShare-function ), but have switchable set
    /// to false, such that we dont recalculate shares.

    if (!switchables.empty() && input["agents"].getChildren().at(0)("switchable") ? input["agents"].getChildren().at(
            0)["switchable"].getBool() : false) {
        if (!input("sharecalculatorsettings")) {
            input.addChild("sharecalculatorsettings");
        }
        shareCalculator = new ShareCalculator(switchables, *variableContainer->price,
                                              input["sharecalculatorsettings"]("strategy")
                                              ? input["sharecalculatorsettings"]["strategy"].getString() : "pdivpf",
                                              input["sharecalculatorsettings"]("alpha_n")
                                              ? input["sharecalculatorsettings"]["alpha_n"].getDouble() : 1.79,
                                              input["sharecalculatorsettings"]("alpha_0")
                                              ? input["sharecalculatorsettings"]["alpha_0"].getDouble() : 0.8);
    } else
        shareCalculator = nullptr;

    //excessDemandCalculator
    excessDemandCalculator = ExcessDemandCalculator::factory(input, agents,
                                                             variableContainer->excessDemand,
                                                             variableContainer->price, variableContainer->dividend);

    //priceCalculator
    priceCalculator = PriceCalculator::factory(input, excessDemandCalculator,
                                               variableContainer->price, variableContainer->excessDemand,
                                               variableContainer->deltaT, randomNumberPool,
                                               agents);

    //DataCollector
    dataCollector = DataCollector::factory(input, variableContainer->price, variableContainer->excessDemand,
                                           agents, switchables);

    //Quantities of Interest
    quantitiesOfInterest = QuantitiesOfInterest::factory(input, variableContainer->price,
                                                         variableContainer->excessDemand,
                                                         agents, switchables, dataCollector);


    //Configure Stock Exchange
    stockExchange = StockExchange::factory(dataCollector, agents,
                                           randomNumberPool, priceCalculator,
                                           excessDemandCalculator,
                                           shareCalculator,
                                           variableContainer->dividend,
                                           variableContainer->globalNews);

    stockExchange->checkInitilisation();

    numSteps = input["numSteps"].getSizeT();

}


/** Do all steps before the simulation. Here: Generate the neighbourhood and collect the initial
 * values with the DataCollector
 *
 */
void Simulation::preSimulation() {
    // In order to capture initial states (notably prices), data collectors
    // must be invoked at the beginning of the simulation.
    dataCollector->collect();
}


/** Run the Simulation for numSteps steps.
 */
bool Simulation::runSimulation(bool &aborted, size_t id) {
    if (aborted)
        return false;

    // 200 segments should be a good upper bound for the progress bar.
    // Assume 400 units to be safe.
    // therefore: numSteps/divisor == 400
    auto startTime = Clock::now();

    bool result = true;


#if WITH_PROGBAR
    std::size_t divisor = numSteps/400;
    if(divisor == 0)
        // divisor = 1 might put too much load on the output buffer, 5 is fine, too.
        divisor = 10;

    std::string progbar_name = "Simulation: " + std::to_string(id);
    progressbar *prog = progressbar_new(progbar_name.c_str() , numSteps/divisor);
#else
    int process = 0;
#if WITH_OPENMP
    process = omp_get_thread_num();
#pragma omp critical (PROGBAR_UPDATE)
#endif
    {
        std::cout << "This is process " << process << " running Simulation " << id << std::endl;
    }
#endif

    for (std::size_t i = 0; i < numSteps; i++) {
        stockExchange->preStep();
        try {
            stockExchange->step();
        }
        catch (MathError &ex) {
            stockExchange->postStep();

            std::cerr << "error in simulation" << endl;
            std::cerr << ex.what() << endl;

            //throw ex;

            result = false;
            aborted = true;
        }
        catch (...) {
            result = false;
            aborted = true;
        }

        stockExchange->postStep();

#if WITH_PROGBAR
#if WITH_OPENMP
#pragma omp critical (PROGBAR_UPDATE)
#endif
        {
            if((i % divisor) == 0)
                progressbar_inc(prog);
        }
#endif

        if (aborted)
            break;
    }

#if WITH_PROGBAR
    #if WITH_OPENMP
        #pragma omp critical (PROGBAR_UPDATE)
    #endif

    progressbar_finish(prog);
#endif

    auto endTime = Clock::now();

    simulationTime =
            static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) *
            1e-9;

    return result;
}


/** Do all postSimulation clean up and maybe calls to analysis classes.
 * Do a final write with the DataCollector.
 */
void Simulation::postSimulation() {

    quantitiesOfInterest->calculateQoi();

    //Delete everything that is not needed for writing to disk
    for (auto& agent : *agents) {
        delete agent;
        agent = nullptr;
    }
    agents->clear();
    delete agents;
    agents = nullptr;

    delete stockExchange;
    stockExchange = nullptr;


    //saveguard casting
    RandomNumberPool *pool = dynamic_cast<RandomNumberPool *>(randomNumberPool);
    if (pool) {
        pool->clearPools();
    }

    delete priceCalculator;
    priceCalculator = nullptr;
    delete excessDemandCalculator;
    excessDemandCalculator = nullptr;

    delete variableContainer;
    variableContainer = nullptr;

    if (shareCalculator != nullptr) {
        delete shareCalculator;
        shareCalculator  = nullptr;
    }

    for (auto& switchable : switchables) {
        delete switchable;
        switchable  = nullptr;
    }
    switchables.clear();


}


/** Destructor. Delete all objects.
 */
Simulation::~Simulation() {

    delete quantitiesOfInterest;
    quantitiesOfInterest = nullptr;
    if (dataCollector != nullptr) {
        dataCollector->deleteDataItemCollectors();
        delete dataCollector;
        dataCollector=nullptr;
    }
    delete randomNumberPool;
    randomNumberPool = nullptr;
}


bool Simulation::executeSimulations(Input::fromFile inputs, bool &aborted) {
    bool ret = true;
    int reps = 0; //Counter for Filenames. Used in writer class
    std::vector<Simulation *> *simulationsToWrite = new std::vector<Simulation *>;

#if WITH_OPENMP
    if (inputs.numThreads > omp_get_max_threads()){
        std::cerr << "Maximum of " << omp_get_max_threads() << " used insted of requested " << inputs.numThreads << std::endl;
    }
    int threads = std::min(inputs.numThreads, omp_get_max_threads());
    omp_set_num_threads(threads);

#pragma omp parallel shared(simulationsToWrite, reps, inputs)
#endif
    {
#if WITH_OPENMP
#pragma omp for schedule(dynamic, 1)
#endif
        for (size_t i = 0; i < inputs.inputs.size(); i++) {
            if (aborted)
                i=inputs.inputs.size()+10;// Hier müsste ein break stehen, aber das geht nicht wegen openmp, daher brechen wir über den Schleifenindex ab ...

            Simulation *simulation = new Simulation();
            simulation->parse(inputs.inputs.at(i));
            simulation->preSimulation();
            simulation->runSimulation(aborted, i);
            simulation->postSimulation();

#if WITH_OPENMP
#pragma omp critical (ADD_SIMULATION)
#endif
            {
                simulationsToWrite->push_back(simulation);
            }
            if (simulationsToWrite->size() > inputs.simulationsPerFile-1) {
                std::vector<Simulation *> *simulationsToWrite2 = simulationsToWrite;
                Writer *writer = nullptr;
#if WITH_OPENMP
#pragma omp critical (ADD_SIMULATION)
#endif
                {

                    simulationsToWrite = new std::vector<Simulation *>;
                    writer = Writer::factory(inputs, reps);
                    int process = 0;
#if WITH_OPENMP
                    process = omp_get_thread_num();
#pragma omp critical (PROGBAR_UPDATE)
#endif
                    {
                        std::cout << "This is process " << process << " writing file " << reps << std::endl;
                    }
                    reps++;
                }

                Simulation::write(simulationsToWrite2, writer);
                for(auto& simulation: *simulationsToWrite2){
                    delete simulation;
                    simulation = nullptr;
                }
                simulationsToWrite2->clear();
                delete simulationsToWrite2;
                simulationsToWrite2 = nullptr;
                delete writer;
                writer = nullptr;
            }


        }
    }
    if (!simulationsToWrite->empty()) {
        Writer *writer = Writer::factory(inputs, reps);
        reps++;
        Simulation::write(simulationsToWrite, writer);
        delete writer;
    }
    for(std::size_t i=0; i<simulationsToWrite->size(); i++){
        delete simulationsToWrite->at(i);
    }
    simulationsToWrite->clear();
    delete simulationsToWrite;
    return ret;
}

void Simulation::write(Writer *writer) {
    writer->addSimulation(input("outputname") ? input["outputname"].getString() : std::string("simulation"));
    writer->saveTime(simulationTime);
    writer->saveBuildInfo();

    std::size_t uniformGenerated = 0;
    std::size_t uniformUnused = 0;
    std::size_t normalGenerated = 0;
    std::size_t normalUnused = 0;
    int seed = 0;

    //saveguard casting
    RandomNumberPool *pool = dynamic_cast<RandomNumberPool *>(randomNumberPool);
    if (pool) {
        pool->getUsageInformation(uniformGenerated, uniformUnused, normalGenerated, normalUnused, seed);
    }

    writer->rngInformation(uniformGenerated, uniformUnused, normalGenerated, normalUnused, seed);

    quantitiesOfInterest->write(writer);

    writer->saveInput(input);

}

void Simulation::write(std::vector<Simulation *> *simulations, Writer *writer) {
    for (auto simulation: *simulations) {
        simulation->write(writer);
    }
}