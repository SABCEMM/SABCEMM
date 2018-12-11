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
 * @file AgentEMB.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <iostream>
#include <stdexcept>
#include "DataItemCollectorFWSharesChartist.h"

using namespace std;


void DataItemCollectorFWSharesChartist::checkInitilisation() {
    assert(chartist != nullptr);
}

void DataItemCollectorFWSharesChartist::collectData() {
    dataMatrix[0].push_back(chartist->getShare());
}

void DataItemCollectorFWSharesChartist::setAgents(std::vector<Agent*>* agents) {
    if (agents->size() != 2) {
        throw invalid_argument("You must specify exactly two agents to track with DataItemCollectorFWShares");
    }

    for(Agent* a : *agents)
    {
        auto aAsChartist = dynamic_cast<AgentFWChartist*>(a);
        auto aAsFundamentalist = dynamic_cast<AgentFWFundamentalist*>(a);
        if(aAsChartist != nullptr)
        {
            chartist = aAsChartist;
            continue;
        }
        if(aAsFundamentalist != nullptr)
        {
            continue;
        }
        throw invalid_argument("DataItemCollectorFWShares only supports agents that derive from AgentFW.");
    }

}

DataItemCollectorFWSharesChartist::DataItemCollectorFWSharesChartist(){
    dataMatrix.clear();
    std::vector<double> temp;
    dataMatrix.push_back(temp);
}