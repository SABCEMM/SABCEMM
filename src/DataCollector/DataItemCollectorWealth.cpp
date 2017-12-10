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
 * @file DataItemCollectorWealth.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <vector>

#include "../Agent/Agent.h"
#include "DataItemCollectorWealth.h"


DataItemCollectorWealth::DataItemCollectorWealth() : DataItemCollectorWealth(Util::DataItemCollectorMethod::MEAN) {

}

DataItemCollectorWealth::DataItemCollectorWealth(Util::DataItemCollectorMethod method):
		DataItemCollector(){
	agents = nullptr;
	wealthHistory.clear();
	wealthHistoryDetail.clear();

	this->method = method;

	price = nullptr;
}

DataItemCollectorWealth::~DataItemCollectorWealth() = default;

void DataItemCollectorWealth::collectData(){

	assert(agents != nullptr);
	assert(price!=nullptr);

	//Calculate amount of cash in the market

	std::vector<double> tempWealth;

	for (auto &agent : *agents) {
		if(agent->hasGroup(groupToTrack_)) {
			tempWealth.push_back(agent->getCash() + agent->getStock() * price->getPrice() );
		}
	}

	switch(method){
		case Util::DataItemCollectorMethod::MEAN:
			wealthHistory.push_back(Util::mean(tempWealth));
			break;
		case Util::DataItemCollectorMethod::STD:
			wealthHistory.push_back(Util::std(tempWealth));
			break;
		case Util::DataItemCollectorMethod::DETAIL:
			wealthHistoryDetail.push_back(tempWealth);
	}

}
void DataItemCollectorWealth::write(){
	assert(writer != nullptr);

	switch(method){
		case Util::DataItemCollectorMethod::DETAIL:
			writer->matrixToFile(&wealthHistoryDetail, name, groupToTrack_, method);
			break;
		default:
			writer->vectorToFile(&wealthHistory, name, groupToTrack_, method);
	}
}
void DataItemCollectorWealth::clearData(){
	wealthHistory.clear();
    wealthHistoryDetail.clear();
}
void DataItemCollectorWealth::checkInitilisation(){

	assert(agents!=nullptr);
	assert(price!=nullptr);

}

void DataItemCollectorWealth::setAgents(std::vector<Agent*>* newAgents){

	agents = newAgents;

}
void DataItemCollectorWealth::setPrice(Price* newPrice){

	price = newPrice;

}
