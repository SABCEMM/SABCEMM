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

#include "VariableContainer.h"
#include "../Parameter/Parameter.h"
#include "DeltaT.h"
#include "Dividend.h"
#include "ExcessDemand.h"
#include "GlobalNews.h"
#include "Price.h"
#include "../RandomGenerator/RandomGenerator.h"

VariableContainer* VariableContainer::factory(Parameter* parameter, RandomGenerator* randomNumberPool){
    VariableContainer* variableContainer = new VariableContainer;

    variableContainer->price = new Price();
    variableContainer->price->setPrice(*(parameter->startPrice));
    variableContainer->excessDemand = new ExcessDemand();

    variableContainer->deltaT = new DeltaT(*(parameter->deltaT));

    if(parameter->parameterSetDividend.Z1){
        variableContainer->dividend = new Dividend(randomNumberPool, variableContainer->deltaT,
                                                   *(parameter->parameterSetDividend.Z1),
                                                   *(parameter->parameterSetDividend.Z2),
                                                   *(parameter->parameterSetDividend.initialDividend));

    }
    else {
        variableContainer->dividend = nullptr;
    }

    variableContainer->globalNews = nullptr;
    for (auto &i : parameter->parameterSetAgent) {
        //If Harras Agents are used create globalNews, if multiple harras agent groups
        // are present last globale news will dominate.
        if (*(i.type) == "AgentHarras") {
            variableContainer->globalNews = new GlobalNews(randomNumberPool);
            //stockExchange->setGlobalNews(globalNews);
        }
    }

    return variableContainer;
}

VariableContainer::VariableContainer(){
    price = nullptr;
    excessDemand = nullptr;
    dividend = nullptr;
    globalNews = nullptr;
}

VariableContainer::~VariableContainer(){
    delete price;
    delete excessDemand;
    delete dividend;
    delete globalNews;
}