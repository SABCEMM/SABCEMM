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
#include <cstddef>
#include "gtest/gtest.h"
#include <vector>

#include "../../src/Agent/Agent.h"
#include "../../src/DataCollector/DataItemCollectorAmountOfCash.h"
#include "../MockClasses/MockAgent.h"
#include "../MockClasses/MockWriter.h"



TEST(DataItemCollectorAmountOfCash, collectDataMean){
	std::vector<Agent*> agents;
	double newCash=2;
	for (int i=0; i<16; i++)
	{	
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setCash(newCash);	
        agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorAmountOfCash collector(DataItemCollector::Method::MEAN);
	collector.setAgents(&agents);

	collector.collectData();

	EXPECT_EQ(collector.dataMatrix.at(0).back(), 2);
	
	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
    }
}

TEST(DataItemCollectorAmountOfCash, collectDataStd){
    std::vector<Agent*> agents;
    for (int i=0; i<16; i++)
    {
        MockAgent* tempAgent = new MockAgent;
        double tempCash = i;
        tempAgent->setCash(tempCash);
        agents.push_back(static_cast<Agent*>(tempAgent));
    }
    DataItemCollectorAmountOfCash collector(DataItemCollector::Method::STD);
    collector.setAgents(&agents);

    collector.collectData();

    EXPECT_NEAR(collector.dataMatrix.at(0).back(), 4.760952285695233, 0.2);

    for (std::size_t i=0; i<16; i++)
    {
        delete agents[i];
    }
}

TEST(DataItemCollectorAmountOfCash, collectData_with1Group){
	std::vector<Agent*> agents;
	double newCash=2;
	std::vector<int> groups;
	groups.push_back(2);
	for (int i=0; i<16; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setCash(newCash);
		tempAgent->setGroups(groups);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorAmountOfCash collector;
	collector.setAgents(&agents);
	collector.setGroupToTrack(2);

	collector.collectData();

	EXPECT_EQ(collector.dataMatrix.at(0).back(), 2);

	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
	}
}

TEST(DataItemCollectorAmountOfCash, collectData_with2Group){
	std::vector<Agent*> agents;
	double newCash=2;
	std::vector<int> groups1;
	groups1.push_back(2);
	std::vector<int> groups2;
	groups2.push_back(3);
	for (int i=0; i<8; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setCash(newCash);
		tempAgent->setGroups(groups1);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	for (int i=0; i<8; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setCash(newCash);
		tempAgent->setGroups(groups2);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorAmountOfCash collector;
	collector.setAgents(&agents);
	collector.setGroupToTrack(2);

	collector.collectData();

	EXPECT_EQ(collector.dataMatrix.at(0).back(), 2);

	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
	}
}



TEST(DataItemCollectorAmountOfCash, checkInitilisation){
    std::vector<Agent*> newAgents;

    DataItemCollectorAmountOfCash dataItemCollectorAmountOfCash;

    EXPECT_EQ(nullptr, dataItemCollectorAmountOfCash.agents);

    dataItemCollectorAmountOfCash.setAgents(&newAgents);

    EXPECT_NO_THROW(dataItemCollectorAmountOfCash.checkInitilisation());
}


TEST(DataItemCollectorAmountOfCash, setAgents){
    std::vector<Agent*> newAgents;

    for(int i = 0;i < 16; i++){
        MockAgent* tempAgent = new MockAgent();
        newAgents.push_back(static_cast<Agent*>(tempAgent));
    }

    DataItemCollectorAmountOfCash dataItemCollectorAmountOfCash;

    EXPECT_EQ(nullptr, dataItemCollectorAmountOfCash.agents);

    dataItemCollectorAmountOfCash.setAgents(&newAgents);

    EXPECT_EQ(newAgents, *dataItemCollectorAmountOfCash.agents);
    EXPECT_EQ(&newAgents, dataItemCollectorAmountOfCash.agents);
    EXPECT_EQ(newAgents.size(), dataItemCollectorAmountOfCash.agents->size());

    for(std::size_t i = 0;i < 16; i++){
        delete newAgents.at(i);
    }
}