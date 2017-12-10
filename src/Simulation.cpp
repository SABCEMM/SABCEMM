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
#include <boost/optional.hpp>
#include <cstddef> //for std::size_t

#include "Simulation.h"

#include "Exceptions/MathError.h"

#include "RandomGenerator/RandomGenerator.h"

#include "StockExchange/StockExchange.h"


#if WITH_PROGBAR
#include "progressbar/progressbar.h"
#endif





using namespace boost;
using namespace std;


/** Standard constructor
 */
Simulation::Simulation() {

	parameter = nullptr;

	stockExchange = nullptr;
	excessDemandCalculator = nullptr;
	dataCollector = nullptr;
	priceCalculator = nullptr;
	writer = nullptr;
	numSteps = 0;
	variableContainer= nullptr;
	randomNumberPool = nullptr;


}



/** Parse the parameter object, create all objects and connect them.
 *	Requires a lot of computation time due to all the if clauses.
 */
void Simulation::parse(Parameter* parameter) {


    this->parameter = parameter;

    assert(parameter->numSteps);

    //Writer
    writer = Writer::factory(parameter);

	//RandomGenerator
	randomNumberPool = RandomGenerator::factory(parameter, writer);


	//Variable Containers
	variableContainer = VariableContainer::factory(parameter, randomNumberPool);

	//Agents
	agents = Agent::factory(parameter, randomNumberPool, variableContainer->price,
							variableContainer->excessDemand, variableContainer->globalNews,
							variableContainer->dividend, variableContainer->deltaT);

	//excessDemandCalculator
	excessDemandCalculator = ExcessDemandCalculator::factory(parameter, agents,
															 variableContainer->excessDemand,
															 variableContainer->price, variableContainer->dividend);

	//priceCalculator
	priceCalculator = PriceCalculator::factory(parameter, excessDemandCalculator,
											   variableContainer->price, variableContainer->excessDemand,
											   variableContainer->deltaT, randomNumberPool,
                                               agents);

	//DataCollector
	dataCollector = DataCollector::factory(parameter, variableContainer->price, variableContainer->excessDemand,
                                           agents, writer);


	//Configure Stock Exchange
    stockExchange = StockExchange::factory(dataCollector, agents,
                                           randomNumberPool, priceCalculator,
                                           excessDemandCalculator, variableContainer->dividend,
										   variableContainer->globalNews);

	stockExchange->checkInitilisation();

	numSteps = *(parameter->numSteps);

	writer->saveInput(parameter);
	writer->saveBuildInfo();

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
void Simulation::runSimulation() {

    // 200 segments should be a good upper bound for the progress bar.
    // Assume 400 units to be safe.
    // therefore: numSteps/divisor == 400
    auto startTime = Clock::now();


    #if WITH_PROGBAR
    std::size_t divisor = numSteps/400;
    if(divisor == 0)
        // divisor = 1 might put too much load on the output buffer, 5 is fine, too.
        divisor = 5;

    progressbar *prog = progressbar_new("Simulating", numSteps/divisor);
    #endif

	for(std::size_t i = 0; i < numSteps; i++)
	{
		stockExchange->preStep();
        try
        {
            stockExchange->step();
        }
        catch (MathError& ex)
        {
            stockExchange->postStep();
            #if WITH_PROGBAR
            progressbar_finish(prog);
            #endif


            std::cerr << "error in simulation" << endl;

            return;
        }

		stockExchange->postStep();
        #if WITH_PROGBAR
        if((i % divisor) == 0)
            progressbar_inc(prog);
        #endif
	}
    #if WITH_PROGBAR
    progressbar_finish(prog);
    #endif

    auto endTime = Clock::now();

    double simulationTime = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) * 1e-9;

    writer->saveTime(simulationTime);

}


/** Do all postSimulation clean up and maybe calls to analysis classes.
 * Do a final write with the DataCollector.
 */
void Simulation::postSimulation() {

	dataCollector->write();

}


/** Destructor. Delete all objects.
 */
Simulation::~Simulation() {

	for (auto &agent : *agents) {
		delete agent;
	}

    delete agents;


	delete stockExchange;


        //if(parameter->parameterSetRNG.enablePool && *(parameter->parameterSetRNG.enablePool)){
        delete randomNumberPool;


    if (dataCollector != nullptr) {
        dataCollector->deleteDataItemCollectors();
       delete dataCollector;
    }


	delete priceCalculator;


	delete variableContainer;

    delete writer;




}


bool Simulation::executeSimulations(vector<Parameter*> parameterSets){
    bool ret = true;

	for (auto &parameterSet : parameterSets) {

		if(!parameterSet->validate()){

            ret = false;
			break;
		}
		Simulation simulation;
		simulation.parse(parameterSet);
		simulation.preSimulation();
		simulation.runSimulation();
		simulation.postSimulation();

		delete parameterSet;
	}

    return ret;
}

bool Simulation::executeSimulations(Parameter* parameterSet){
    vector<Parameter*> parameterSets;
    parameterSets.push_back(parameterSet);

    return Simulation::executeSimulations(parameterSets);
}
