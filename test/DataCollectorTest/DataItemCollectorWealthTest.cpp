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

#include "../../src/Agent/Agent.h"
#include "../../src/DataCollector/DataItemCollectorWealth.h"

#include "../MockClasses/MockAgent.h"
#include "../MockClasses/MockWriter.h"
#include <cstddef> //for std::size_t


TEST(DataItemCollectorWealth, collectData){
    vector<Agent*> agents;
	double newStock=2;
	double newCash=2;
	Price* newPrice = new Price;
	newPrice->setPrice(1);
	for (int i=0; i<16; i++)
	{	
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setStock(newStock);
		tempAgent->setCash(newCash);	
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorWealth collector;
	collector.setPrice(newPrice);
	collector.setAgents(&agents);

	collector.collectData();

	EXPECT_EQ(collector.wealthHistory.back(), 4);

	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
	}
    delete newPrice;
}

TEST(DataItemCollectorWealth, collectData_with1Group){
    vector<Agent*> agents;
    double newStock=2;
    double newCash=2;
    Price* newPrice = new Price;
    newPrice->setPrice(1);
	vector<int> groups(1,2);

	for (int i=0; i<16; i++)
	{
        MockAgent* tempAgent = new MockAgent;
        tempAgent->setStock(newStock);
        tempAgent->setCash(newCash);
        tempAgent->setGroups(groups);
        agents.push_back(static_cast<Agent*>(tempAgent));
	}
    DataItemCollectorWealth collector;
    collector.setGroupToTrack(2);
    collector.setPrice(newPrice);
    collector.setAgents(&agents);

    collector.collectData();

    EXPECT_EQ(collector.wealthHistory.back(), 4);

    for (std::size_t i=0; i<16; i++)
    {
        delete agents[i];
    }
    delete newPrice;
}

TEST(DataItemCollectorWealth, collectData_with2Group){
    vector<Agent*> agents;
    double newStock=2;
    double newCash=2;
    Price* newPrice = new Price;
    newPrice->setPrice(2);
	vector<int> groups1(1,2);
	vector<int> groups2(1,3);

	for (int i=0; i<8; i++)
	{
        MockAgent* tempAgent = new MockAgent;
        tempAgent->setStock(newStock);
        tempAgent->setCash(newCash);
		tempAgent->setGroups(groups1);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
    newStock += 3;
    newCash -= 1;
	for (int i=0; i<8; i++)
	{
        MockAgent* tempAgent = new MockAgent;
        tempAgent->setStock(newStock);
        tempAgent->setCash(newCash);
		tempAgent->setGroups(groups2);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
    DataItemCollectorWealth collector;
    collector.setGroupToTrack(2);
    collector.setPrice(newPrice);
    collector.setAgents(&agents);

    collector.collectData();

    EXPECT_EQ(collector.wealthHistory.back(), 6);

    for (std::size_t i=0; i<16; i++)
    {
        delete agents[i];
    }
    delete newPrice;
}


TEST(DataItemCollectorWealth, write){
    MockWriter mockWriter;
    DataItemCollectorWealth dataItemCollectorWealth;
    dataItemCollectorWealth.setWriter(&mockWriter);

    EXPECT_CALL(mockWriter, vectorToFile(testing::_,testing::_,testing::_,testing::_)).Times(1);

    dataItemCollectorWealth.write();
}

TEST(DataItemCollectorWealth, clearData){
    DataItemCollectorWealth dataItemCollectorWealth;
    vector<double> testWealth;
    testWealth.push_back(2);
    testWealth.push_back(3);
    testWealth.push_back(4);

    dataItemCollectorWealth.wealthHistory = testWealth;

    EXPECT_EQ(testWealth.size(), dataItemCollectorWealth.wealthHistory.size());

    dataItemCollectorWealth.clearData();

    EXPECT_EQ( 0, dataItemCollectorWealth.wealthHistory.size());
}

TEST(DataItemCollectorWealth, checkInitilisation){
    std::vector<Agent*> newAgents;
    Price price;

    DataItemCollectorWealth dataItemCollectorWealth;

    EXPECT_EQ(nullptr, dataItemCollectorWealth.agents);
    EXPECT_EQ(nullptr, dataItemCollectorWealth.price);

    ASSERT_DEATH(dataItemCollectorWealth.checkInitilisation(), ".*");

    dataItemCollectorWealth.setAgents(&newAgents);
    ASSERT_DEATH(dataItemCollectorWealth.checkInitilisation(), ".*");

    dataItemCollectorWealth.setPrice(&price);

    EXPECT_NO_THROW(dataItemCollectorWealth.checkInitilisation());
}


TEST(DataItemCollectorWealth, setAgents){
    std::vector<Agent*> newAgents;

    for(std::size_t i = 0;i < 16; i++){
        MockAgent* tempAgent = new MockAgent();
        newAgents.push_back(static_cast<Agent*>(tempAgent));
    }

    DataItemCollectorWealth dataItemCollectorWealth;

    EXPECT_EQ(nullptr, dataItemCollectorWealth.agents);

    dataItemCollectorWealth.setAgents(&newAgents);

    EXPECT_EQ(newAgents, *dataItemCollectorWealth.agents);
    EXPECT_EQ(&newAgents, dataItemCollectorWealth.agents);
    EXPECT_EQ(newAgents.size(), dataItemCollectorWealth.agents->size());

    for(std::size_t i = 0;i < 16; i++){
        delete newAgents.at(i);
    }
}