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
#include "../../src/DataCollector/DataItemCollectorAmountOfStock.h"
#include "../MockClasses/MockAgent.h"
#include "../MockClasses/MockWriter.h"
#include <cstddef> //for std::size_t


TEST(DataItemCollectorAmountOfStock, collectData){
	std::vector<Agent*> agents;
	double newStock=2;
	for (int i=0; i<16; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setStock(newStock);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorAmountOfStock collector;
	collector.setAgents(&agents);

	collector.collectData();

	EXPECT_EQ(collector.amountOfStock.back(), 2);

	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
	}
}

TEST(DataItemCollectorAmountOfStock, collectData_with1Group){
	std::vector<Agent*> agents;
	double newStock=2;
	vector<int> groups(1,2);

	for (int i=0; i<16; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setStock(newStock);
		tempAgent->setGroups(groups);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorAmountOfStock collector;
	collector.setAgents(&agents);
	collector.setGroupToTrack(2);

	collector.collectData();

	EXPECT_EQ(collector.amountOfStock.back(), 2);

	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
	}
}

TEST(DataItemCollectorAmountOfStock, collectData_with2Group){
	std::vector<Agent*> agents;
	double newStock=2;
	vector<int> groups1(1,2);
	vector<int> groups2(1,3);

	for (int i=0; i<8; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setStock(newStock);
		tempAgent->setGroups(groups1);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}

	newStock += 2;
	for (int i=0; i<8; i++)
	{
		MockAgent* tempAgent = new MockAgent;
		tempAgent->setStock(newStock);
		tempAgent->setGroups(groups2);
		agents.push_back(static_cast<Agent*>(tempAgent));
	}
	DataItemCollectorAmountOfStock collector;
	collector.setAgents(&agents);
	collector.setGroupToTrack(2);

	collector.collectData();

	EXPECT_EQ(collector.amountOfStock.back(), 2);

	for (std::size_t i=0; i<16; i++)
	{
		delete agents[i];
	}
}


TEST(DataItemCollectorAmountOfStock, write){
	MockWriter mockWriter;
	DataItemCollectorAmountOfStock dataItemCollectorAmountOfStock;
	dataItemCollectorAmountOfStock.setWriter(&mockWriter);

	EXPECT_CALL(mockWriter, vectorToFile(testing::_,testing::_,testing::_,testing::_)).Times(1);

	dataItemCollectorAmountOfStock.write();
}

TEST(DataItemCollectorAmountOfStock, clearData){
	DataItemCollectorAmountOfStock dataItemCollectorAmountOfStock;
	vector<double> testStock;
	testStock.push_back(2);
	testStock.push_back(3);
	testStock.push_back(4);

	dataItemCollectorAmountOfStock.amountOfStock = testStock;

	EXPECT_EQ(testStock.size(), dataItemCollectorAmountOfStock.amountOfStock.size());

	dataItemCollectorAmountOfStock.clearData();

	EXPECT_EQ( 0, dataItemCollectorAmountOfStock.amountOfStock.size());
}
TEST(DataItemCollectorAmountOfStock, checkInitilisation){
	std::vector<Agent*> newAgents;

	DataItemCollectorAmountOfStock dataItemCollectorAmountOfStock;

	EXPECT_EQ(nullptr, dataItemCollectorAmountOfStock.agents);

	dataItemCollectorAmountOfStock.setAgents(&newAgents);

	EXPECT_NO_THROW(dataItemCollectorAmountOfStock.checkInitilisation());
}


TEST(DataItemCollectorAmountOfStock, setAgents){
	std::vector<Agent*> newAgents;

	for(int i = 0;i < 16; i++){
		MockAgent* tempAgent = new MockAgent();
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	DataItemCollectorAmountOfStock dataItemCollectorAmountOfStock;

	EXPECT_EQ(nullptr, dataItemCollectorAmountOfStock.agents);

	dataItemCollectorAmountOfStock.setAgents(&newAgents);

	EXPECT_EQ(newAgents, *dataItemCollectorAmountOfStock.agents);
	EXPECT_EQ(&newAgents, dataItemCollectorAmountOfStock.agents);
	EXPECT_EQ(newAgents.size(), dataItemCollectorAmountOfStock.agents->size());

	for(std::size_t i = 0;i < 16; i++){
		delete newAgents.at(i);
	}
}