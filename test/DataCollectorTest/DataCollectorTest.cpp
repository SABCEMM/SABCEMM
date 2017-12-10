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
#include <cstddef> //for std::size_t
#include "gtest/gtest.h"

#include "../src/DataCollector/DataCollector.h"
#include "../MockClasses/MockDataItemCollector.h"


//Creating a Test Fixture since the setup for all tests is pretty equal

class DataCollectorTest: public ::testing::Test{
public:
	DataCollector* dataCollector;
	std::vector<MockDataItemCollector*> mockDataItemCollectorVector;
	std::size_t numDataItemCollectors = 16;
    bool alreadyDeleted=false;
	
	DataCollectorTest(){
		dataCollector = new DataCollector;
		for(std::size_t i=0; i<numDataItemCollectors; i++){
			auto * mockDataItemCollector = new MockDataItemCollector;
			mockDataItemCollectorVector.push_back(mockDataItemCollector);
			dataCollector->components.push_back(static_cast<DataItemCollector*>(mockDataItemCollector));
		}
	}
	~DataCollectorTest(){
		delete dataCollector;
		if(!alreadyDeleted){
			for(std::size_t i=0; i<numDataItemCollectors; i++){
				delete mockDataItemCollectorVector.at(static_cast<unsigned long>(i));
			}
		}
	}
};



TEST_F(DataCollectorTest, checkInitilisation){
	for(std::size_t i=0; i<numDataItemCollectors; i++){
		EXPECT_CALL(*(mockDataItemCollectorVector.at(i)), checkInitilisation()).Times(1);
	}
	dataCollector->checkInitilisation();
}

TEST_F(DataCollectorTest, write){
	for(std::size_t i=0; i<numDataItemCollectors; i++){
		EXPECT_CALL(*(mockDataItemCollectorVector.at(i)), write()).Times(1);
	}
	dataCollector->write();
}

TEST_F(DataCollectorTest, collect){
	for(std::size_t i=0; i<numDataItemCollectors; i++){
		EXPECT_CALL(*(mockDataItemCollectorVector.at(i)), collectData()).Times(1);
	}
	dataCollector->collect();
}

TEST_F(DataCollectorTest, clearData){
	for(std::size_t i=0; i<numDataItemCollectors; i++){
		EXPECT_CALL(*(mockDataItemCollectorVector.at(i)), checkInitilisation()).Times(1);
	}
	dataCollector->checkInitilisation();
}

TEST_F(DataCollectorTest, add){
	EXPECT_EQ(dataCollector->components.size(), numDataItemCollectors);

	MockDataItemCollector* mockDataItemCollector = new MockDataItemCollector;
	dataCollector->add(static_cast<DataItemCollector*>(mockDataItemCollector));
	EXPECT_EQ(dataCollector->components.size(), numDataItemCollectors+1);
	EXPECT_EQ(dataCollector->components.back(), mockDataItemCollector);

	delete mockDataItemCollector;
}

TEST_F(DataCollectorTest, deleteDataItemCollectors){
	EXPECT_EQ(dataCollector->components.size(), numDataItemCollectors);
	dataCollector->deleteDataItemCollectors();
	EXPECT_EQ(dataCollector->components.size(), 0);

	// Avoid deleting twice in the destructor of the testfixture
	alreadyDeleted = true;
}
