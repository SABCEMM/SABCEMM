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

#include "../../src/VariableContainer/Price.h"
#include "../../src/DataCollector/DataItemCollectorPrice.h"

#include "../MockClasses/MockWriter.h"


TEST(DataItemCollectorPrice, collectData){

	Price* newPrice = new Price;
	newPrice->setPrice(5);

	DataItemCollectorPrice collector(newPrice);
	collector.collectData();

	EXPECT_EQ(collector.priceHistory.back(), 5);	
	
	delete newPrice;
}

TEST(DataItemCollectorPrice, write){
	MockWriter mockWriter;
	DataItemCollectorPrice dataItemCollectorPrice;
	dataItemCollectorPrice.setWriter(&mockWriter);

	EXPECT_CALL(mockWriter, vectorToFile(testing::_,testing::_,testing::_)).Times(1);

	dataItemCollectorPrice.write();
}
TEST(DataItemCollectorPrice, clearData){
	DataItemCollectorPrice dataItemCollectorPrice;
	vector<double> testPrice;
	testPrice.push_back(2);
	testPrice.push_back(3);
	testPrice.push_back(4);

	dataItemCollectorPrice.priceHistory = testPrice;

	EXPECT_EQ(testPrice.size(), dataItemCollectorPrice.priceHistory.size());
	EXPECT_EQ(testPrice, dataItemCollectorPrice.priceHistory);

	dataItemCollectorPrice.clearData();

	EXPECT_EQ( 0, dataItemCollectorPrice.priceHistory.size());
}
TEST(DataItemCollectorPrice, checkInitilisation){

	DataItemCollectorPrice dataItemCollectorPrice;
	EXPECT_DEATH(dataItemCollectorPrice.checkInitilisation(), ".*");

	Price newPrice;
	DataItemCollectorPrice dataItemCollectorPrice1(&newPrice);
	EXPECT_NO_THROW(dataItemCollectorPrice1.checkInitilisation());
}