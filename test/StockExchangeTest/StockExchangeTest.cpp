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

#include "../../src/Agent/AgentHarras.h"
#include "../../src/Agent/AgentCross.h"
#include "../../src/PriceCalculator/PriceCalculatorCross.h"
#include "../../src/Simulation.h"
#include "../../src/StockExchange/StockExchange.h"

#include "../DummyClasses/RandomGeneratorFunctional.h"
#include "../MockClasses/MockAgent.h"
#include "../MockClasses/MockPriceCalculator.h"
#include "../MockClasses/MockDataCollector.h"
#include "../MockClasses/MockDataItemCollector.h"

#include <cstddef> //for std::size_t


using ::testing::AtLeast;  //For Google Mock: Why necessary?
using namespace std;

TEST(stockExchange, preStep){
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    GlobalNews globalNews(randomGenerator);
    globalNews.curGlobalNews = 5;
    globalNews.prevGlobalNews = 10;
    MockPriceCalculator priceCalculator;
    std::vector<Agent*> agents;
    StockExchange stockExchange(nullptr, &agents, nullptr, &priceCalculator, nullptr, nullptr, nullptr);

    EXPECT_CALL(priceCalculator, preStepCalculate()).Times(1);

    for(int i = 0;i < 16; i++){
        MockAgent* tempAgent = new MockAgent();
        EXPECT_CALL(*tempAgent, preStepUpdate()).Times(1);
        agents.push_back(static_cast<Agent*>(tempAgent));
    }


    stockExchange.setGlobalNews(&globalNews);


    stockExchange.preStep();

    EXPECT_EQ(globalNews.curGlobalNews, 1);
    EXPECT_EQ(globalNews.prevGlobalNews, 5);

    delete randomGenerator;
    for(std::size_t i=0; i<16; i++){
        delete agents.at(i);
    }
}



TEST(stockExchange, step){

    MockPriceCalculator priceCalculator;
    std::vector<Agent*> agents;
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    StockExchange stockExchange(nullptr, &agents, randomGenerator, &priceCalculator, nullptr, nullptr, nullptr);

    EXPECT_CALL(priceCalculator, stepCalculate()).Times(2);

    for(int i = 0;i < 16; i++){
        MockAgent* tempAgent = new MockAgent();
        EXPECT_CALL(*tempAgent, stepUpdate()).Times(2);
        agents.push_back(static_cast<Agent*>(tempAgent));
    }

    stockExchange.step();

    vector<std::size_t> agentIndex1 = stockExchange.agentIndex;

    stockExchange.step();

    //Check if agent ordner is different than in last step. One diference in the calling order is sufficient for
    // the test to pass.
    bool test = true;

    int indexSum=0;

    for(std::size_t i=0; i<16; i++){
        indexSum += static_cast<int>(stockExchange.agentIndex.at(i));
        if(stockExchange.agentIndex.at(i) != agentIndex1.at(i)) test=false;
        delete agents.at(i);
    }

    EXPECT_EQ(stockExchange.agentIndex.size(), 16);
    EXPECT_EQ(indexSum, 15*16/2); //Gaußsche Summe 0-15
    EXPECT_EQ(test, false);

    delete randomGenerator;
}

TEST(stockExchange, postStep){
    MockDataCollector dataCollector;
    MockDataItemCollector dataItemCollector1, dataItemCollector2, dataItemCollector3;
    MockPriceCalculator priceCalculator;
    std::vector<Agent*> agents;

    dataCollector.add(&dataItemCollector1);
    dataCollector.add(&dataItemCollector2);
    dataCollector.add(&dataItemCollector3);
    EXPECT_CALL(dataItemCollector1, collectData()).Times(1);
    EXPECT_CALL(dataItemCollector2, collectData()).Times(1);
    EXPECT_CALL(dataItemCollector3, collectData()).Times(1);

    EXPECT_CALL(priceCalculator, postStepCalculate()).Times(1);

    for(int i = 0;i < 16; i++){
        MockAgent* tempAgent = new MockAgent();
        EXPECT_CALL(*tempAgent, postStepUpdate()).Times(1);
        agents.push_back(static_cast<Agent*>(tempAgent));
    }

    StockExchange* stockExchange = StockExchange::factory();

    stockExchange->setAgents(&agents);
    stockExchange->setDataCollector(&dataCollector);
    stockExchange->setPriceCalculator(&priceCalculator);


    stockExchange->postStep();

    for(std::size_t i=0; i<16; i++){
        delete agents.at(i);
    }
}

