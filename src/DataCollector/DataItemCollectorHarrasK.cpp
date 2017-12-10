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
 * @file DataItemCollectorHarrasK.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "DataItemCollectorHarrasK.h"


DataItemCollectorHarrasK::DataItemCollectorHarrasK() : DataItemCollectorHarrasK(Util::DataItemCollectorMethod::MEAN, nullptr) {

}

/** Constructor for the DataItemCollectorHarrasK. Requires a pointer to an array of AgentHarras.
 */
DataItemCollectorHarrasK::DataItemCollectorHarrasK(std::vector<Agent*>* newAgents):DataItemCollectorHarrasK(Util::DataItemCollectorMethod::MEAN, newAgents) {

}

DataItemCollectorHarrasK::DataItemCollectorHarrasK(Util::DataItemCollectorMethod method, std::vector<Agent*>* newAgents) {
	agents = (std::vector<AgentHarras*>*)newAgents;
	kHistory.clear();
	kHistoryDetail.clear();

	this->method = method;
}


/** Standard destructor
 */
DataItemCollectorHarrasK::~DataItemCollectorHarrasK() = default;


/** Clears all data in the history.
 */
void DataItemCollectorHarrasK::clearData(){

	kHistory.clear();
	kHistoryDetail.clear();

}


/** Writes the kHistory.
 */
void DataItemCollectorHarrasK::write(){

	assert(writer != nullptr);

    switch(method){
        case Util::DataItemCollectorMethod::DETAIL:
            writer->matrixToFile(&kHistoryDetail, name, groupToTrack_, method);
            break;
        default:
            writer->vectorToFile(&kHistory, name, groupToTrack_, method);
    }

}


/** Check if everything is initialized and ready to run.
 */
void DataItemCollectorHarrasK::checkInitilisation(){
	assert(agents!=nullptr);
}


/** Calculate the current average trust of all agents in their neighbours and save to history.
 */
void DataItemCollectorHarrasK::collectData(){
	assert(agents != nullptr);

	std::vector<double> tempK;

	for (auto &agent : *agents) {
		if(agent->hasGroup(groupToTrack_)) {
			std::vector<double> tempKvector = agent->getK(); //< Get the vector of trust coefficients
			double meanK = Util::mean(tempKvector);
			tempK.push_back(meanK);
		}
	}

	switch(method){
		case Util::DataItemCollectorMethod::MEAN:
			kHistory.push_back(Util::mean(tempK));
			break;
		case Util::DataItemCollectorMethod::STD:
			kHistory.push_back(Util::std(tempK));
			break;
		case Util::DataItemCollectorMethod::DETAIL:
			kHistoryDetail.push_back(tempK);
	}
}


void DataItemCollectorHarrasK::setAgents(std::vector<Agent*>* newAgents){
	agents = (std::vector<AgentHarras*>*)newAgents;
}