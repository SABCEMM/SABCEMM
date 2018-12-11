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

#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../src/DataCollector/DataItemCollector.h"
#include "../MockClasses/MockDataItemCollector.h"
#include "../MockClasses/MockWriter.h"


TEST(DataItemCollector, collect){

	MockDataItemCollector mockCollector;

	mockCollector.setCollectInterval(5);

	EXPECT_CALL(mockCollector, collectData()).Times(2);
	
	for(int i=0; i<10; i++){
		mockCollector.collect();
	}

}

TEST(DataItemCollector, collect1){

	MockDataItemCollector mockCollector;

	mockCollector.setCollectInterval(1);
	EXPECT_CALL(mockCollector, collectData()).Times(10);

	for(int i=0; i<10; i++){
		mockCollector.collect();
	}

}

TEST(DataItemCollector, setName){
    MockDataItemCollector mockCollector;

    std::string testName = "TestName";
    mockCollector.setName(testName);
    EXPECT_EQ(mockCollector.name, testName);
}

TEST(DataItemCollector, getName){
    MockDataItemCollector mockCollector;

    std::string testName = "TestName";
    mockCollector.setName(testName);
    EXPECT_EQ(mockCollector.getName(), testName);
}


TEST(DataItemCollector, setCollectInterval){
    MockDataItemCollector mockCollector;
    int collectInterval = 123;
    mockCollector.setCollectInterval(collectInterval);
    EXPECT_EQ(collectInterval, mockCollector.collectInterval);
}
TEST(DataItemCollector, setGroupToTrack){
    MockDataItemCollector mockCollector;
    int groupToTrack = 3;
    mockCollector.setGroupToTrack(groupToTrack);
    EXPECT_EQ(groupToTrack, mockCollector.groupToTrack_);
}

TEST(DataItemCollector, clearData){
    MockDataItemCollector dataItemCollector;
    std::vector<std::vector<double>> dataMatrix;
    std::vector<double> dataVector;
    dataVector.push_back(2);
    dataVector.push_back(3);
    dataVector.push_back(4);
    dataMatrix.push_back(dataVector);
    dataVector.push_back(6);
    dataVector.push_back(7);
    dataMatrix.push_back(dataVector);

    dataItemCollector.dataMatrix = dataMatrix;

    EXPECT_EQ(dataMatrix.size(), dataItemCollector.dataMatrix.size());
    EXPECT_EQ(dataMatrix.at(0).size(), dataItemCollector.dataMatrix.at(0).size());
    EXPECT_EQ(dataMatrix.at(1).size(), dataItemCollector.dataMatrix.at(1).size());

    dataItemCollector.clearData();

    EXPECT_EQ(0, dataItemCollector.dataMatrix.size());
}