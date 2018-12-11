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
#include <vector>
#include <cstddef> //for std::size_t
#include "../../src/Agent/Agent.h"
#include "../../src/Agent/AgentHarras.h"
#include "../../src/DataCollector/DataItemCollectorHarrasK.h"
#include "../MockClasses/MockAgent.h"
#include "../MockClasses/MockWriter.h"
#include "../DummyClasses/RandomGeneratorFunctional.h"



TEST(DataItemCollectorHarrasK, collectData){
    std::vector<Agent*> agents;
    std::vector<double> trust (4,1);

    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    ExcessDemand excessDemand;
    GlobalNews globalNews(randomGenerator);
    Price price;

    for (int i=0; i<16; i++)
    {
        AgentHarras* tempAgent = new AgentHarras(randomGenerator, &price, &excessDemand, &globalNews);
        tempAgent->k = trust;
        tempAgent->countNeighbours = 4;
        agents.push_back(tempAgent);
    }
    DataItemCollectorHarrasK collector(&agents);

    collector.collectData();

    EXPECT_EQ(collector.dataMatrix.at(0).back(), 1);

    for (std::size_t i=0; i<16; i++)
    {
        delete agents[i];
    }
    delete randomGenerator;
}



TEST(DataItemCollectorHarrasK, collectData_with1Group){
    std::vector<Agent*> agents;
    std::vector<double> trust;

    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    ExcessDemand excessDemand;
    GlobalNews globalNews(randomGenerator);
    Price price;

    for(int i=0; i<4; i++)
    {
        trust.push_back(2);
    }

    std::vector<int> groups;
    groups.push_back(2);
    for (int i=0; i<16; i++)
    {
        AgentHarras* tempAgent = new AgentHarras(randomGenerator, &price, &excessDemand, &globalNews);
        tempAgent->setGroups(groups);
        tempAgent->k = trust;
        tempAgent->countNeighbours = 4;
        agents.push_back(tempAgent);
    }


    DataItemCollectorHarrasK collector;
    collector.setAgents(&agents);
    collector.setGroupToTrack(2);

    collector.collectData();

    EXPECT_EQ(collector.dataMatrix.at(0).back(), 2);

    for (std::size_t i=0; i<16; i++)
    {
        delete agents[i];
    }

    delete randomGenerator;
}

TEST(DataItemCollectorHarrasK, collectData_with2Group){
    std::vector<Agent*> agents;
    std::vector<double> trust1 (4,2); // four double with value 2
    std::vector<double> trust2 (4,3); // four double with value 3

    std::vector<int> groups1 (1,2);
    std::vector<int> groups2 (1,3);

    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    ExcessDemand excessDemand;
    GlobalNews globalNews(randomGenerator);
    Price price;

    for (int i=0; i<8; i++)
    {
        AgentHarras* tempAgent = new AgentHarras(randomGenerator, &price, &excessDemand, &globalNews);
        tempAgent->setGroups(groups1);
        tempAgent->k = trust1;
        tempAgent->countNeighbours = 4;
        agents.push_back(tempAgent);
    }
    for (int i=0; i<8; i++)
    {
        AgentHarras* tempAgent = new AgentHarras(randomGenerator, &price, &excessDemand, &globalNews);
        tempAgent->setGroups(groups2);
        tempAgent->k = trust2;
        tempAgent->countNeighbours = 4;
        agents.push_back(tempAgent);
    }

    DataItemCollectorHarrasK collector;
    collector.setAgents(&agents);
    collector.setGroupToTrack(2);

    collector.collectData();

    EXPECT_EQ(collector.dataMatrix.at(0).back(), 2);

    for (std::size_t i=0; i<16; i++)
    {
        delete agents[i];
    }

    delete randomGenerator;
}



TEST(DataItemCollectorHarrasK, checkInitilisation){
    std::vector<Agent*> newAgents;

    DataItemCollectorHarrasK dataItemCollectorHarrasK;

    EXPECT_EQ(nullptr, dataItemCollectorHarrasK.agents);

    dataItemCollectorHarrasK.setAgents(&newAgents);

    EXPECT_NO_THROW(dataItemCollectorHarrasK.checkInitilisation());
}


TEST(DataItemCollectorHarrasK, setAgents){
    std::vector<Agent*> newAgents;

    for(int i = 0;i < 16; i++){
        MockAgent* tempAgent = new MockAgent();
        newAgents.push_back(static_cast<Agent*>(tempAgent));
    }

    DataItemCollectorHarrasK dataItemCollectorHarrasK;

    EXPECT_EQ(nullptr, dataItemCollectorHarrasK.agents);

    dataItemCollectorHarrasK.setAgents(&newAgents);

    //EXPECT_EQ(newAgents, *dataItemCollectorHarrasK.agents);
    //EXPECT_EQ(&newAgents, dataItemCollectorHarrasK.agents);
    EXPECT_EQ(newAgents.size(), dataItemCollectorHarrasK.agents->size());

    for(std::size_t i = 0;i < 16; i++){
        delete newAgents.at(i);
    }
}
