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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../src/Agent/AgentCross.h"
#include "../../src/Agent/AgentFW.h"
#include "../../src/Agent/AgentHarras.h"
#include "../../src/Agent/AgentLLS.h"
#include "../../src/PriceCalculator/PriceCalculatorCross.h"
#include "../../src/Input/Input.h"
#include "../../src/Simulation.h"
#include "../../src/StockExchange/StockExchange.h"

#include "../DummyClasses/RandomGeneratorFunctional.h"

#include "../../src/RandomGenerator/RandomGeneratorFromFile.h"
#include "../MockClasses/MockAgent.h"
#include "../MockClasses/MockPriceCalculator.h"
#include "../MockClasses/MockDataCollector.h"
#include "../MockClasses/MockDataItemCollector.h"
#include "../../src/Agent/AgentFWFundamentalist.h"
#include "../../src/Agent/AgentFWChartist.h"

#include <cstddef> //for std::size_t


using ::testing::AtLeast;  //For Google Mock: Why necessary?
using namespace std;


/// values are read from file to spot errors that are in normal initialization eventually.
/// test setup:
/// 9 agents with C1,C2,C3,threshold assigned equidistantly.
///
///
/// globalNews are taken from one run of the matlab simulation.
///
/// The respective states of the C++- and MATLAB-simulation are compared after each step.
TEST(fullSimulationTest, fullSimulation_Harras){

    const double tol = 0.000001;
    const double eps = 1e-14;

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_Harras.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);


//    const std::size_t numSteps = input["numSteps"].getSizeT();
    const double marketDepth = input["priceCalculatorSettings"]["marketDepth"].getDouble();

    EXPECT_TRUE(input["agents"].hasChild("AgentHarras"));
    const std::size_t numAgents = input["agents"]["AgentHarras"]["count"].getSizeT();

    Simulation simulation;
    simulation.parse(input);

    const double Tc1[] = { 0.1, 1, 16, 3 };
    const double Tc2[] = { 12, 2, 1, 3 };
    const double Tc3[] = { 3, 0.5, 0.5, 3 };
    const double Tthreshold[] = { 0.5, 0.5, 0.5, 0.5 };
    const int Tdecision[] = { 1, -1, 1, 0 };
    const double Tvolume[] = { 0.02, 0.02, 0.02, 0.02 };
    const double TglobalNews[] = { -1, 0 };

    const int TdecisionNeighbours[][4] =
    {
        {  1, -1,  0,  0 },
        {  0,  1, -1,  0 },
        {  0, -1,  1,  0 },
        {  1,  1,  1,  1 }
    };

    const double Tk[][4] =   {
        { -1,  0,  0,  1 },
        { -1, -1,  0,  0 },
        {  0, -1, -1,  0 },
        {  0,  0,  1, -1 }
    };


    for(std::size_t i = 0; i < numAgents; i++)
    {
        AgentHarras* asHarras = dynamic_cast<AgentHarras*>(simulation.agents->at(i));
        asHarras->c1 = Tc1[i];
        asHarras->c2 = Tc2[i];
        asHarras->c3 = Tc3[i];
        asHarras->threshold = Tthreshold[i];
        asHarras->decision = Tdecision[i];
        asHarras->tradingVolume = Tvolume[i];
        asHarras->setRandomGenerator(new RandomGeneratorFunctional(ConstantReturn(1)));
    }

    simulation.preSimulation();

    // generate neighbourhood.
    for(std::size_t i = 0; i < numAgents; i++)
    {
        AgentHarras* asHarras = dynamic_cast<AgentHarras*>(simulation.agents->at(i));
        for(std::size_t j = 0; j < 4; j++)
            asHarras->decisionNeighbours.at(j) = TdecisionNeighbours[i][j];
        for(std::size_t j = 0; j < 4; j++)
            asHarras->k.at(j) = Tk[i][j];
    }

        // -- simulation.runSimulation() (adapted)
        // Calculate one timestep by hand
        std::size_t i = 0;
        // -- stockExchange->preStep() (adapted)
        StockExchange* exchange = dynamic_cast<StockExchange*>(simulation.stockExchange);
        GlobalNews* news = exchange->globalNews;

        EXPECT_TRUE(news != nullptr);

        news->prevGlobalNews = TglobalNews[i];
        news->curGlobalNews = TglobalNews[i+1];

        // -- stockExchange->step() (adapted)

        exchange->priceCalculator->stepCalculate();
        // conflicting definitions:
        // the division by lambda, which is done in the paper, in not done in
        //  C++ (on purpose).
        EXPECT_NEAR(0.01, simulation.variableContainer->excessDemand->getExcessDemand()/marketDepth, tol);
        EXPECT_NEAR(exp(0.01), simulation.variableContainer->price->getPrice(), tol);

        // no shuffling(!)
        for(std::size_t j = 0;j < exchange->agents->size(); j++)
            exchange->agents->at(j)->stepUpdate();

        simulation.stockExchange->postStep();



        // reference numbers according to PDF
        /// @todo PDF ins Wiki hochladen

        // common for all agents
        /// @todo die Berechnung von u ist für alle Agenten gleich - dennoch wird u für jede Instanz
        ///       neu berechnet. Besser einmal berechnen und für alle Instanzen setzen!
        ///       Oder gleich als klassenweite statische Variable setzen.
        for(std::size_t j = 0; j < exchange->agents->size(); j++)
        {
            AgentHarras a = *dynamic_cast<AgentHarras*>(exchange->agents->at(i));
            EXPECT_NEAR(a.kr, 0, eps);
            EXPECT_NEAR(a.sigma, sqrt(0.0095), tol);
            EXPECT_NEAR(a.u, -0.05*0.01/sqrt(0.0095), tol);
        }

        // agent 0
        AgentHarras a0 = *dynamic_cast<AgentHarras*>(exchange->agents->at(0));
        EXPECT_NEAR(2-0.02*exp(0.01), a0.getCash(), tol);
        EXPECT_NEAR(2+0.02, a0.getStock(), tol);

        EXPECT_NEAR(-0.95+0.05*0.01/sqrt(0.0095), a0.k.at(0), tol);
        EXPECT_NEAR(-0.05*0.01/sqrt(0.0095), a0.k.at(1), tol);
        EXPECT_NEAR(0, a0.k.at(2), eps);
        EXPECT_NEAR(0.95, a0.k.at(3), tol);

        EXPECT_NEAR(3.05, a0.opinion, tol);
        EXPECT_NEAR(1, a0.decision, eps);
        EXPECT_NEAR(0.4*(2-0.02*exp(0.01))/exp(0.01), a0.tradingVolume, tol);

        // agent 1
        AgentHarras a1 = *dynamic_cast<AgentHarras*>(exchange->agents->at(1));
        EXPECT_NEAR(2+0.02*exp(0.01), a1.getCash(), tol);
        EXPECT_NEAR(2-0.02, a1.getStock(), tol);

        EXPECT_NEAR(-0.95, a1.k.at(0), tol);
        EXPECT_NEAR(-0.95+0.05*0.01/sqrt(0.0095), a1.k.at(1), tol);
        EXPECT_NEAR(-0.05*0.01/sqrt(0.0095), a1.k.at(2), tol);
        EXPECT_NEAR(0, a1.k.at(3), eps);

        EXPECT_NEAR(0, a1.opinion, tol);
        EXPECT_NEAR(0, a1.decision, eps);
        EXPECT_NEAR(0, a1.tradingVolume, eps);

        // agent 2
        AgentHarras a2 = *dynamic_cast<AgentHarras*>(exchange->agents->at(2));
        EXPECT_NEAR(2-0.02*exp(0.01), a2.getCash(), tol);
        EXPECT_NEAR(2+0.02, a2.getStock(), tol);

        EXPECT_NEAR(0, a2.k.at(0), eps);
        EXPECT_NEAR(-0.95-0.05*0.01/sqrt(0.0095), a2.k.at(1), tol);
        EXPECT_NEAR(-0.95+0.05*0.01/sqrt(0.0095), a2.k.at(2), tol);
        EXPECT_NEAR(0, a2.k.at(3), eps);

        EXPECT_NEAR(-3.5, a2.opinion, tol);
        EXPECT_NEAR(-1, a2.decision, eps);
        EXPECT_NEAR(0.4*2.02, a2.tradingVolume, tol);

        // agent 3
        AgentHarras a3 = *dynamic_cast<AgentHarras*>(exchange->agents->at(3));
        EXPECT_NEAR(2, a3.getCash(), eps);
        EXPECT_NEAR(2, a3.getStock(), eps);

        EXPECT_NEAR(0.05*0.01/sqrt(0.0095), a3.k.at(0), tol);
        EXPECT_NEAR(0.05*0.01/sqrt(0.0095), a3.k.at(1), tol);
        EXPECT_NEAR(0.95+0.05*0.01/sqrt(0.0095), a3.k.at(2), tol);
        EXPECT_NEAR(-0.95+0.05*0.01/sqrt(0.0095), a3.k.at(3), tol);

        EXPECT_NEAR(3, a3.opinion, tol);
        EXPECT_NEAR(1, a3.decision, eps);
        EXPECT_NEAR(0.4*2/exp(0.01), a3.tradingVolume, tol);

    //Timestep End

    simulation.postSimulation();
}


TEST(fullSimulationTest, fullSimulation_Cross){
    const double tol = 0.000001;
    const double eps = 1e-14;

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_Cross.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);


    // @TODO: Zeile unnötig?
    //const double marketDepth = *(param.parameterSetPriceCalculator.marketDepth);

    EXPECT_EQ(input["agents"].getChildren().at(0).getName(), "agentcross");
    const std::size_t numAgents = input["agents"]["agentCross"]["count"].getSizeT();

    Simulation simulation;
    simulation.parse(input);

    const double cowardiceThreshold[] = { 0.1, 0.3, 0.5, 0.7 };
    const double herdingThreshold[] = { 0.001, 0.002, 0.003, 0.004 };
    const double initialHerdingPressure[] = { 0.0005, 0.001, 0.002, 0.001 };
    const int initialDecision[] = { 1, -1, -1, -1 };
    const double cowardiceLowerBounds[] = {1/1.1, 1/1.3, 1/1.5, 1/1.7 };
    const double cowardiceUpperBounds[] = {1.1, 1.3, 1.5, 1.7 };


    for(std::size_t i = 0; i < numAgents; i++)
    {
        AgentCross* asCross = dynamic_cast<AgentCross*>(simulation.agents->at(i));
        asCross->herdingThreshold = herdingThreshold[i];
        asCross->inactionThreshold = cowardiceThreshold[i];
        asCross->herdingPressure = initialHerdingPressure[i];
        asCross->decision = initialDecision[i];

        /// @todo bounds automatisch updaten, wenn Preis sich geändert hat (im Setter)?
        asCross->updateBounds(1);

        EXPECT_NEAR(asCross->inactionLowerBound, cowardiceLowerBounds[i], eps);
        EXPECT_NEAR(asCross->inactionUpperBound, cowardiceUpperBounds[i], eps);
    }

    simulation.preSimulation();

    StockExchange* exchange = dynamic_cast<StockExchange*>(simulation.stockExchange);

    PriceCalculatorCross* pcc = dynamic_cast<PriceCalculatorCross*>(exchange->priceCalculator);
    // contents of the list will be drawn as "random" global news.
    pcc->randomGenerator = new RandomGeneratorFunctional(ListReturn({0.3}));

    // -- simulation.runSimulation() (adapted)
    // Run one timestep per hand
        // -- stockExchange->preStep() (adapted)

        ///@todo Verwendung von globalNews einbauen? Bisher wird sqrt(h)*eta im priceCalculator
        ///      zufällig bestimmt, dabei beschreibt dieser Term globale Info im Modell.

        // -- stockExchange->step() (adapted)

        exchange->priceCalculator->stepCalculate();
        // p(1) (analytically)
        const double price_1 = exp((0.3)*(1+0.3*2./4)+0.2*(-2./4));
        EXPECT_NEAR(simulation.variableContainer->price->getPrice(), price_1, tol);

        // no shuffling(!)
        for(std::size_t j = 0;j < exchange->agents->size(); j++)
            exchange->agents->at(j)->stepUpdate();


        // common for all agents
        for(std::size_t j = 1; j < exchange->agents->size(); j++)
        {
            AgentCross a = *dynamic_cast<AgentCross*>(exchange->agents->at(j));
            EXPECT_NEAR(a.herdingPressure, initialHerdingPressure[j], eps);
            EXPECT_EQ(a.decision, -1);
        }

        // agent 0
        AgentCross a0 = *dynamic_cast<AgentCross*>(exchange->agents->at(0));
        EXPECT_NEAR(a0.herdingPressure, 0, eps);
        EXPECT_EQ(a0.decision, -1);
        EXPECT_NEAR(a0.inactionLowerBound, price_1/(1+0.1), tol);
        EXPECT_NEAR(a0.inactionUpperBound, price_1*(1+0.1), tol);

        // agent 1
        AgentCross a1 = *dynamic_cast<AgentCross*>(exchange->agents->at(1));
        EXPECT_NEAR(a1.inactionLowerBound, 1/(1+0.3), tol);
        EXPECT_NEAR(a1.inactionUpperBound, 1*(1+0.3), tol);

        // agent 2
        AgentCross a2 = *dynamic_cast<AgentCross*>(exchange->agents->at(2));
        EXPECT_NEAR(a2.inactionLowerBound, 1/(1+0.5), tol);
        EXPECT_NEAR(a2.inactionUpperBound, 1*(1+0.5), tol);

        // agent 3
        AgentCross a3 = *dynamic_cast<AgentCross*>(exchange->agents->at(3));
        EXPECT_NEAR(a3.inactionLowerBound, 1/(1+0.7), tol);
        EXPECT_NEAR(a3.inactionUpperBound, 1*(1+0.7), tol);


        simulation.stockExchange->postStep();

        // Timestep end

    simulation.postSimulation();
}

TEST(fullSimulationTest, fullSimulation_RII){
    const double tol = 0.05; // due to numerical difficulties we are extremely generous in this test.

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_RII.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);



    const size_t numSteps = input["numsteps"].getSizeT();

    EXPECT_EQ(input["agents"].getChildren().at(0).getName(), "agentlls");
    EXPECT_TRUE(input["agents"]["agentlls"].hasChild("agentRII"));
    const std::size_t numAgents = input["agents"]["agentlls"]["agentRII"]["count"].getSizeT();

    Simulation simulation;
    simulation.parse(input);
    simulation.randomNumberPool = new RandomGeneratorFunctional(ListReturn({0.015}));

    const double epsilon[] = { 0.0341, -0.0209 };

    for(size_t i = 0; i < numAgents; i++)
    {
        AgentLLS* asLLS = dynamic_cast<AgentLLS*>(simulation.agents->at(i));
        // P(0) - initialization should be fine however.
        //asEMB->oldPrice = 50;
        ASSERT_EQ(asLLS->strategy, AgentLLS::LLSStrategy::RII);
        asLLS->setRandomGenerator(new RandomGeneratorFunctional(ListReturn({epsilon[i]})));
    }

    simulation.preSimulation();

    StockExchange* exchange = dynamic_cast<StockExchange*>(simulation.stockExchange);
    Dividend* dividend = simulation.stockExchange->dividend;


    dividend->randomGenerator = new RandomGeneratorFunctional(ConstantReturn(0.015));

    // -- simulation.runSimulation() (adapted)
    for(size_t i = 0; i < numSteps; i++)
    {
        // -- stockExchange->preStep() (adapted)
        dividend->calculateDividend();
        exchange->excessDemandCalculator->preStepCalculate();
        exchange->priceCalculator->preStepCalculate();

        EXPECT_NEAR(dividend->getDividend(), 0.5075, tol);

        ///@todo Verwendung von globalNews einbauen? Bisher wird sqrt(h)*eta im priceCalculator
        ///      zufällig bestimmt, dabei beschreibt dieser Term globale Info im Modell.

        // -- stockExchange->step() (adapted)

        exchange->priceCalculator->stepCalculate();

        EXPECT_NEAR(simulation.variableContainer->price->getPrice(), 21.1945, tol);

        AgentLLS* a1 = dynamic_cast<AgentLLS*>(exchange->agents->at(0));
        AgentLLS* a2 = dynamic_cast<AgentLLS*>(exchange->agents->at(1));

        // X(1) (gamma without noise and in bounds [0, 0.99]) for the current timestep.
        EXPECT_NEAR(a1->iterGamma, 0.2959, tol);
        EXPECT_NEAR(a1->hypoCash, 722.02, tol);
        EXPECT_NEAR(a2->iterGamma, 0.2959, tol);
        EXPECT_NEAR(a2->hypoCash, 722.02, tol);
        // or check against whatever upper bound is given in the input file.
        EXPECT_TRUE(simulation.variableContainer->excessDemand->getExcessDemand() < 0.1);

        // noise is applied.
        // no shuffling(!)
        for(size_t j = 0;j < exchange->agents->size(); j++)
            exchange->agents->at(j)->stepUpdate();

        // X*(1) (gamma with noise and in bounds [0, 0.99]) for the current timestep.
        EXPECT_NEAR(a1->gamma, 0.2959+epsilon[0], tol);
        EXPECT_NEAR(a2->gamma, 0.2959+epsilon[1], tol);


        // -- stockExchage->postStep() (adapted)
        /// @todo maybe do not adapt the function but just call it?
        exchange->excessDemandCalculator->postStepCalculate();

        exchange->priceCalculator->postStepCalculate();

        // gamma optimization is very sensitive .. we allow for a huge error margin.
        // the price after noise must be greater than the price before!
        EXPECT_NEAR(simulation.variableContainer->price->getPrice(), 22.1215, 0.3);

        for (auto &agent : *exchange->agents) {
            agent->postStepUpdate();
        }

        /// @todo check trading volume

        EXPECT_NEAR(a1->cash, 731.29, 0.01*731.29);
        EXPECT_NEAR(a2->cash, 731.29, 0.01*731.29);

        simulation.stockExchange->postStep();
    }

    simulation.postSimulation();
}

TEST(fullSimulationTest, fullSimulation_EMB){
    const double tol = 0.05; // due to numerical difficulties we are extremely generous in this test.

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_EMB.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);


    const size_t numSteps = input["numsteps"].getSizeT();

    EXPECT_EQ(input["agents"].getChildren().at(0).getName(), "agentlls");
    const std::size_t numAgents = input["agents"]["agentlls"]["AgentEMB"]["count"].getSizeT();



    Simulation simulation;
    simulation.parse(input);
    simulation.randomNumberPool = new RandomGeneratorFunctional(ListReturn({0.015}));

    const double epsilon[] = { -0.095, 0.0029 };
    const vector<double> history[] = { {0.039, 0.043}, {0.034, 0.045} };

    for(size_t i = 0; i < numAgents; i++)
    {
        AgentLLS* asLLS = dynamic_cast<AgentLLS*>(simulation.agents->at(i));

        ASSERT_EQ(asLLS->strategy, AgentLLS::LLSStrategy::EMB);
        asLLS->setRandomGenerator(new RandomGeneratorFunctional(ListReturn({epsilon[i]})));
        asLLS->relPriceHistory = history[i];
    }

    simulation.preSimulation();

    StockExchange* exchange = dynamic_cast<StockExchange*>(simulation.stockExchange);
    Dividend* dividend = simulation.stockExchange->dividend;

    // the dividend's actual RNG should work.

    // -- simulation.runSimulation() (adapted)
    for(size_t i = 0; i < numSteps; i++)
    {
        // -- stockExchange->preStep() (adapted)
        dividend->calculateDividend();
        exchange->excessDemandCalculator->preStepCalculate();
        exchange->priceCalculator->preStepCalculate();

        EXPECT_NEAR(dividend->getDividend(), 0.21, tol);

        ///@todo Verwendung von globalNews einbauen? Bisher wird sqrt(h)*eta im priceCalculator
        ///      zufällig bestimmt, dabei beschreibt dieser Term globale Info im Modell.

        // -- stockExchange->step() (adapted)

        exchange->priceCalculator->stepCalculate();

        EXPECT_NEAR(simulation.variableContainer->price->getPrice(), 6.45, tol);

        AgentLLS* a1 = dynamic_cast<AgentLLS*>(exchange->agents->at(0));
        AgentLLS* a2 = dynamic_cast<AgentLLS*>(exchange->agents->at(1));

        // X(1) (gamma without noise and in bounds [0, 0.99]) for the current timestep.
        // hypoCash is not important?
        EXPECT_NEAR(a1->iterGamma, 0.99, tol);
        EXPECT_NEAR(a2->iterGamma, 0.01, tol);
        // or check against whatever upper bound is given in the input file.
        EXPECT_TRUE(simulation.variableContainer->excessDemand->getExcessDemand() < 0.1);

        // noise is applied.
        // no shuffling(!)
        for(size_t j = 0;j < exchange->agents->size(); j++)
            exchange->agents->at(j)->stepUpdate();

        // X*(1) (gamma with noise and in bounds [0, 0.99]) for the current timestep.
        EXPECT_NEAR(a1->gamma, 0.99+epsilon[0], tol);
        EXPECT_NEAR(a2->gamma, 0.01+epsilon[1], tol);


        // -- stockExchage->postStep() (adapted)
        /// @todo maybe do not adapt the function but just call it?
        exchange->excessDemandCalculator->postStepCalculate();

        exchange->priceCalculator->postStepCalculate();

        // gamma optimization is very sensitive .. we allow for a huge error margin.
        // the price after noise must be greater than the price before!
        ASSERT_NEAR(simulation.variableContainer->price->getPrice(), 5.65, 0.3);

        for (auto &agent : *exchange->agents) {
            agent->postStepUpdate();
        }

        /// @todo check trading volume

        EXPECT_NEAR(a1->cash, 1210, 0.02*1786);
        EXPECT_NEAR(a2->cash, 1210, 0.02*1786);

        simulation.stockExchange->postStep();
    }

    simulation.postSimulation();
}


/**
TEST(fullSimulationTest, fullSimulation_EMB){
    const double tol = 0.000001;
    const double eps = 1e-14;

    // Erstelle agents.
    vector<Parameter*> params = Parameter::readFromFile("StockExchangeTest/fullSimulation_EMB.xml");
    ASSERT_EQ(params.size(), 1);

    Parameter param = *(params.at(0));

    ASSERT_TRUE(param.validate());

    const size_t numSteps = *param.numSteps;

    ASSERT_EQ(param.parameterSetAgent.at(0).type.get(), "AgentEMB");
    const size_t numAgents = *(param.parameterSetAgent.at(0).count);
    const size_t memorySpan = *(param.parameterSetAgent.at(0).memorySpan);

    Simulation simulation;
    simulation.parse(&param);

    // Set initial values.

    const double gamma[] = { 0.9, 0.1 };
    const double epsilon[] = { 0.1, -0.1 };

    // P(1)
    simulation.price->setPrice(1);

    for(size_t i = 0; i < numAgents; i++)
    {
        AgentEMB* asEMB = dynamic_cast<AgentEMB*>(simulation.agents.at(i));
        // P(0)
        asEMB->oldPrice = 0.5;
        // iterGamma is the unprocessed gamma that determines how
        // wealth should be split for the current timestep.
        asEMB->iterGamma = gamma[i];
        asEMB->setRandomGenerator(new RandomGeneratorFunctional(ListReturn({epsilon[i]})));
        /// @todo check that this deque held some random values before clearing it.
        // has been filled with random values that would make our tests tedious.
        // Clearing the deque does not suffice as we need the deque to hold `memorySpan` elements.
        asEMB->relPriceHistory = std::deque<double>(memorySpan, 0);
    }


    simulation.preSimulation();

    StockExchange* exchange = dynamic_cast<StockExchange*>(simulation.stockExchange);
    Dividend* dividend = simulation.stockExchange->dividend;

    dividend->randomGenerator = new RandomGeneratorFunctional(ConstantReturn(0.00015));
    /// @todo diese Initialisierung muss auch außerhalb der Simulationen so erfolgen können!
    dividend->dividend = 0.004;

    // -- simulation.runSimulation() (adapted)
    for(int i = 0; i < numSteps; i++)
    {
        // -- stockExchange->preStep() (adapted)
        dividend->calculateDividend();

        ASSERT_NEAR(dividend->getDividend(), 0.0040006, tol);

        ///@todo Verwendung von globalNews einbauen? Bisher wird sqrt(h)*eta im priceCalculator
        ///      zufällig bestimmt, dabei beschreibt dieser Term globale Info im Modell.

        // -- stockExchange->step() (adapted)

        /// @todo left out as objective function does not have a sensible zero point.
        //exchange->priceCalculator->calculatePrice();

        // no shuffling(!)
        for(int j = 0;j < exchange->agents->size(); j++)
            exchange->agents->at(j)->updateAgent();

        // common for all agents
        for(size_t j = 0; j < exchange->agents->size(); j++)
        {
            AgentEMB a = *dynamic_cast<AgentEMB*>(exchange->agents->at(i));
            ASSERT_NEAR(a.relPriceHistory.back(), 1.0080012, tol);
        }

        // agent 1
        AgentEMB a1 = *dynamic_cast<AgentEMB*>(exchange->agents->at(0));
        // X*(1) (gamma with noise and in bounds [0, 0.99]) for the current timestep.
        ASSERT_NEAR(a1.gamma, 0.99, eps);
        ASSERT_NEAR(a1.cash, 1008.021188, tol);
        // X_h(1) (gamma in bounds [0, 0.99] for the next timestep.
        ASSERT_NEAR(a1.iterGamma, 0.99, eps);
        ASSERT_NEAR(a1.hypoCash, 2010.0553289, tol);
        // iterGamma must maximize the expected utility. Test that the expected utility
        // for gamma=0.99 is correct. Note that as a minimizer is used, expectedUtility
        // is negative.
        ASSERT_NEAR(a1.expectedUtility(0.99), -7.67522325, tol);
        ASSERT_NEAR(a1.tradingVolume, 0.99*2010.0553289/2., tol);

        // agent 2
        AgentEMB a2 = *dynamic_cast<AgentEMB*>(exchange->agents->at(1));
        ASSERT_NEAR(a2.gamma, 0, eps);
        ASSERT_NEAR(a2.cash, 1010, tol);
        ASSERT_NEAR(a2.iterGamma, 0.99, eps);
        ASSERT_NEAR(a2.hypoCash, 1020.1, tol);
        ASSERT_NEAR(a2.expectedUtility(0.99), -6.99696166, tol);
        ASSERT_NEAR(a2.tradingVolume, 0.99*1020.1/2., tol);

        simulation.stockExchange->postStep();
    }

    simulation.postSimulation();
}
*/

TEST(fullSimulationTest, fullSimulation_DCA_HPM){
    const double tol = 1e-6;
    const double eps = 1e-14;

    // taken from one run with the jupyter notebook
    const vector<double> prices =
            {
                1.0,
                1.0076821,
                0.99873766244077222,
                1.0038712901973033,
                1.0023845549373125,
                1.0084223734716686,
                1.0178367487490301,
                1.0097274104261111,
                1.012356809228816,
                1.0185164796299515,
                1.0257380384558286,
                1.0328499162209757,
                1.0365188463195745,
                1.0348176014973671,
                1.0341193348398188
            };

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_FW_DCA-HPM.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);


    EXPECT_EQ(input["agents"].getChildren().at(0).getName(), "agentfw");


    Simulation simulation;
    simulation.parse(input);

    // parameter validation
    AgentFWFundamentalist* fundamentalist = dynamic_cast<AgentFWFundamentalist*>(simulation.agents->at(0));
    AgentFWChartist* chartist = dynamic_cast<AgentFWChartist*>(simulation.agents->at(1));
    for(size_t i = 0 ; i <= 1; ++i) {
        AgentFW *agent = dynamic_cast<AgentFW*>(simulation.agents->at(i));
        EXPECT_NEAR(agent->eta, 0.991, eps);
        EXPECT_NEAR(agent->beta, 1, eps);

        EXPECT_NEAR(agent->alpha_0, -0.327, eps);
        EXPECT_NEAR(agent->alpha_n, 1.79, eps);
        EXPECT_NEAR(agent->alpha_p, 18.43, eps);

        EXPECT_EQ(agent->switchingStrategy, AgentFW::SwitchingStrategy::DCA);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::W]);
        EXPECT_TRUE(agent->indexStrategy[AgentFW::IndexStrategy::H]);
        EXPECT_TRUE(agent->indexStrategy[AgentFW::IndexStrategy::P]);
        EXPECT_TRUE(agent->indexStrategy[AgentFW::IndexStrategy::M]);
    }

    EXPECT_NEAR(fundamentalist->phi, 0.12, eps);
    EXPECT_NEAR(fundamentalist->fundamentalPrice, 1, eps);
    EXPECT_NEAR(fundamentalist->sigma, 0.758, tol);
    EXPECT_NEAR(chartist->chi, 1.5, eps);
    EXPECT_NEAR(chartist->sigma, 2.087, eps);



    // initialize variables which cannot be set in input file.

    /// @todo make this initializible in the input file
    RandomGeneratorFromFile rng("FullSimulationTest/FW_numbers.csv");
    chartist->setRandomGenerator(&rng);
    fundamentalist->setRandomGenerator(&rng);

    const size_t numSteps = simulation.numSteps;

    simulation.preSimulation();

    StockExchange* exchange = simulation.stockExchange;

    // -- simulation.runSimulation() (adapted)
    // Run one timestep per hand
        for(size_t i = 0; i < numSteps; ++i) {
            // -- stockExchange->preStep() (adapted)
            simulation.excessDemandCalculator->preStepCalculate();
            simulation.priceCalculator->preStepCalculate();
            for (std::size_t j = 0; j < exchange->agents->size(); j++)
                exchange->agents->at(j)->preStepUpdate();

            // -- stockExchange->step() (adapted)

            exchange->priceCalculator->stepCalculate();

            // no shuffling(!)
            for (std::size_t j = 0; j < exchange->agents->size(); j++)
                exchange->agents->at(j)->stepUpdate();

            // validate agents

            simulation.stockExchange->postStep();

            EXPECT_NEAR(prices[i], simulation.variableContainer->price->getPrice(), tol);
        }

    // timestep end

    simulation.postSimulation();
}

TEST(fullSimulationTest, fullSimulation_TPA_W){
    const double tol = 1e-6;
    const double eps = 1e-14;

    // taken from one run with the jupyter notebook
    const vector<double> prices=
             {
                1.0,
                1.0076821,
                1.00034359043,
                1.006843593821269,
                1.0012729942228613,
                1.0097336703404738,
                1.0205623741223004,
                1.0135115016939176,
                1.0157769483897359,
                1.0141075169531291,
                1.0169522089579874,
                1.0236861302157265,
                1.0316144876081736,
                1.0254744811916638,
                1.0299064106673297
             };

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_FW_TPA-W.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);



    EXPECT_EQ(input["agents"].getChildren().at(0).getName(), "agentfw");


    Simulation simulation;
    simulation.parse(input);

    // parameter validation
    AgentFWFundamentalist* fundamentalist = dynamic_cast<AgentFWFundamentalist*>(simulation.agents->at(0));
    AgentFWChartist* chartist = dynamic_cast<AgentFWChartist*>(simulation.agents->at(1));
    for(size_t i = 0 ; i <= 1; ++i) {
        AgentFW *agent = dynamic_cast<AgentFW*>(simulation.agents->at(i));
        EXPECT_NEAR(agent->eta, 0.987, eps);
        EXPECT_NEAR(agent->nu, 0.05, eps);
        EXPECT_NEAR(agent->alpha_w, 1041, eps);

        EXPECT_EQ(agent->switchingStrategy, AgentFW::SwitchingStrategy::TPA);
        EXPECT_TRUE(agent->indexStrategy[AgentFW::IndexStrategy::W]);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::H]);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::P]);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::M]);
    }

    EXPECT_NEAR(fundamentalist->phi, 1.15, eps);
    EXPECT_NEAR(fundamentalist->fundamentalPrice, 1, eps);
    EXPECT_NEAR(fundamentalist->sigma, 0.715, tol);
    EXPECT_NEAR(chartist->chi, 0.81, eps);
    EXPECT_NEAR(chartist->sigma, 1.528, eps);




    // initialize variables which cannot be set in input file.

    /// @todo make this initializible in the input file
    RandomGeneratorFromFile rng("FullSimulationTest/FW_numbers.csv");
    chartist->setRandomGenerator(&rng);
    fundamentalist->setRandomGenerator(&rng);

    const size_t numSteps = simulation.numSteps;

    simulation.preSimulation();

    StockExchange* exchange = simulation.stockExchange;

    // -- simulation.runSimulation() (adapted)
    // Run one timestep per hand
    for(size_t i = 0; i < numSteps; ++i) {
        // -- stockExchange->preStep() (adapted)
        simulation.excessDemandCalculator->preStepCalculate();
        simulation.priceCalculator->preStepCalculate();
        for (std::size_t j = 0; j < exchange->agents->size(); j++)
            exchange->agents->at(j)->preStepUpdate();

        // -- stockExchange->step() (adapted)

        exchange->priceCalculator->stepCalculate();

        // no shuffling(!)
        for (std::size_t j = 0; j < exchange->agents->size(); j++)
            exchange->agents->at(j)->stepUpdate();

        // validate agents

        simulation.stockExchange->postStep();

        EXPECT_NEAR(prices[i], simulation.variableContainer->price->getPrice(), tol);
    }

    // timestep end

    simulation.postSimulation();
}

/**TEST(fullSimulationTest, fullSimulation_TPAC_W){
    const double tol = 1e-6;
    const double eps = 1e-14;

    // taken from one run with the jupyter notebook
    const vector<double> prices=
            {
                    1.0,
                    1.0076821,
                    1.00034359043,
                    1.006843593821269,
                    1.0012729942228613,
                    1.0097336703404738,
                    1.0205623741223004,
                    1.0135115016939176,
                    1.0157769483897359,
                    1.0141075169531291,
                    1.0169522089579874,
                    1.0236861302157265,
                    1.0316144876081736,
                    1.0254744811916638,
                    1.0299064106673297
            };

    // Erstelle agents.
    Input::fromFile inputs = Input::readFromFile("FullSimulationTest/fullSimulation_FW_TPAC-W.xml");
    ASSERT_EQ(inputs.inputs.size(), 1);

    Input input = inputs.inputs.at(0);



    EXPECT_EQ(input["agents"].getChildren().at(0).getName(), "agentfw");


    Simulation simulation;
    simulation.parse(input);

    // parameter validation
    AgentFWFundamentalist* fundamentalist = dynamic_cast<AgentFWFundamentalist*>(simulation.agents->at(0));
    AgentFWChartist* chartist = dynamic_cast<AgentFWChartist*>(simulation.agents->at(1));
    for(size_t i = 0 ; i <= 1; ++i) {
        AgentFW *agent = dynamic_cast<AgentFW*>(simulation.agents->at(i));
        EXPECT_NEAR(agent->eta, 0.987, eps);
        EXPECT_NEAR(agent->nu, 0.05, eps);
        EXPECT_NEAR(agent->alpha_w, 1041, eps);

        EXPECT_EQ(agent->switchingStrategy, AgentFW::SwitchingStrategy::TPAC);
        EXPECT_TRUE(agent->indexStrategy[AgentFW::IndexStrategy::W]);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::H]);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::P]);
        EXPECT_FALSE(agent->indexStrategy[AgentFW::IndexStrategy::M]);
    }

    EXPECT_NEAR(fundamentalist->phi, 1.15, eps);
    EXPECT_NEAR(fundamentalist->fundamentalPrice, 1, eps);
    EXPECT_NEAR(fundamentalist->sigma, 0.715, tol);
    EXPECT_NEAR(chartist->chi, 0.81, eps);
    EXPECT_NEAR(chartist->sigma, 1.528, eps);




    // initialize variables which cannot be set in input file.

    /// @todo make this initializible in the input file
    RandomGeneratorFromFile rng("FullSimulationTest/FW_numbers.csv");
    chartist->setRandomGenerator(&rng);
    fundamentalist->setRandomGenerator(&rng);

    const size_t numSteps = simulation.numSteps;

    simulation.preSimulation();

    StockExchange* exchange = simulation.stockExchange;

    // -- simulation.runSimulation() (adapted)
    // Run one timestep per hand
    for(size_t i = 0; i < numSteps; ++i) {
        // -- stockExchange->preStep() (adapted)
        simulation.excessDemandCalculator->preStepCalculate();
        simulation.priceCalculator->preStepCalculate();
        for (std::size_t j = 0; j < exchange->agents->size(); j++)
            exchange->agents->at(j)->preStepUpdate();

        // -- stockExchange->step() (adapted)

        exchange->priceCalculator->stepCalculate();

        // no shuffling(!)
        for (std::size_t j = 0; j < exchange->agents->size(); j++)
            exchange->agents->at(j)->stepUpdate();

        // validate agents

        simulation.stockExchange->postStep();

        EXPECT_NEAR(prices[i], simulation.variableContainer->price->getPrice(), tol);
    }

    // timestep end

    simulation.postSimulation();
}**/
