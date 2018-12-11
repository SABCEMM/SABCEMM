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
 * @file AgentHarras.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <cstddef> //for std::size_t
#include "DataItemCollectorEMBGamma.h"
#include <cmath>

using namespace std;

DataItemCollectorEMBGamma::DataItemCollectorEMBGamma() :
        DataItemCollectorEMBGamma("mean") {

}

DataItemCollectorEMBGamma::DataItemCollectorEMBGamma(DataItemCollector::Method method): method(method){
    agents = nullptr;
    dataMatrix.clear();

    if (!(method == DataItemCollector::Method::MEAN || method == DataItemCollector::Method::EXTREME_PROPORTIONS ||
          method == DataItemCollector::Method::MEAN_NOISE_IMPACT ))
        throw "unsupported data item collector method.";

    std::vector<double> temp;
    dataMatrix.push_back(temp);
}


DataItemCollectorEMBGamma::DataItemCollectorEMBGamma(std::string method): DataItemCollectorEMBGamma(DataItemCollector::stringToDataItemCollectorMethod(method)){

}




/** Check if everything is initialized and ready to run.
 */
void DataItemCollectorEMBGamma::checkInitilisation(){
    assert(agents!=nullptr);
}


/** Calculate the current average trust of all agents in their neighbours and save to history.
 */
void DataItemCollectorEMBGamma::collectData(){
    assert(agents != nullptr);

    std::size_t count = 0;
    std::size_t groupSize = 0;
    double result = 0;

    switch(method)
    {
        case DataItemCollector::Method::MEAN_NOISE_IMPACT:
    {
        double totalGamma = 0;
        for (auto &agent : *agents) {
            if(agent->hasGroup(groupToTrack_)) {
                groupSize++;
                double agentGamma = agent->getIterGamma()-agent->getGamma();
                totalGamma += agentGamma;
            }
            result = static_cast<double>(totalGamma) / static_cast<double>(groupSize);
        }
        break;
    }
    case DataItemCollector::Method::MEAN:
    {
        double totalGamma = 0;
        for (auto &agent : *agents) {
            if(agent->hasGroup(groupToTrack_)) {
                groupSize++;
                double agentGamma = agent->getIterGamma();
                totalGamma += agentGamma;
            }
            result = static_cast<double>(totalGamma) / static_cast<double>(groupSize);
        }
        break;
    }
    case DataItemCollector::Method::EXTREME_PROPORTIONS:
    {
        const double tol = 0.01;
        for (auto &agent : *agents) {
            if(agent->hasGroup(groupToTrack_)) {
                groupSize++;
                double agentGamma = agent->getIterGamma();
                if( fabs(agentGamma-0.01) < tol || fabs(agentGamma-0.99) < tol )
                    ++count;
            }
            result = static_cast<double>(count) / static_cast<double>(groupSize);
        }
        break;
    }
    default:
        throw "invalid collector method " + DataItemCollector::methodToString(method);
    }

    dataMatrix.at(0).push_back(result);
}

void DataItemCollectorEMBGamma::setAgents(std::vector<Agent*>* newAgents){
    agents = (std::vector<AgentLLS*>*)newAgents;
}

vector<vector<double>> * DataItemCollectorEMBGamma::getData(){
    return &dataMatrix;
}