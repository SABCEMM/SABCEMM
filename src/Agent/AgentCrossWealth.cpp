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
 * @file AgentCrossWealth.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <cassert>

#include "AgentCross.h"
#include "AgentCrossWealth.h"


/** Standard constructor of AgentCrossWealth
 */
AgentCrossWealth::AgentCrossWealth():
		AgentCrossWealth(nullptr, nullptr, nullptr, 0, 0){
}


/** Constructor of AgentCrossWealth. Requires a randomGenerator, a Price container,
 *  an ExcessDemand container and a GlobalNews container to work.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newExcessDemand Pointer to the ExcessDemand container
 */
AgentCrossWealth::AgentCrossWealth(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand):
		AgentCrossWealth(newRandomGenerator, newPrice, newExcessDemand, 0, 0){
}


/** Constructor of AgentCrossWealth. Requires a randomGenerator, a Price container, an ExcessDemand container
 *  and a GlobalNews container to work. Also sets the initial cash and stock of the agent.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newExcessDemand Pointer to the ExcessDemand container
 *  \param newCash Initial cash of the agent
 *  \param newStock Initial stock of the agent
 */
AgentCrossWealth::AgentCrossWealth(RandomGenerator* newRandomGenerator, Price* newPrice,
                                   ExcessDemand* newExcessDemand,
                                   double newCash, double newStock):
        AgentCrossWealth(newRandomGenerator, newPrice,
                         newExcessDemand, newCash,
                         newStock, nullptr,
                             0, 0, 0, 0, 0, 0){

}


AgentCrossWealth::AgentCrossWealth(RandomGenerator* newRandomGenerator, Price* newPrice,
                                   ExcessDemand* newExcessDemand, double newCash,
                                   double newStock, DeltaT* deltaT,
                                   double b1, double b2,
                                   double A1, double A2,
                                   double gamma, double interestRate):
        AgentCross(newRandomGenerator, newPrice,
                   newExcessDemand, newCash,
                   newStock, deltaT,
                   b1,  b2,
                   A1,  A2),
        gamma(gamma),
        interestRate(interestRate){
    oldPrice = 1;
}


void AgentCrossWealth::stepUpdate(){
    AgentCross::stepUpdate();
    cash = cash + (1-gamma) * deltaT->getDeltaT() * interestRate * cash
           + gamma * cash * (price->getPrice()-oldPrice)/oldPrice * decision;
    oldPrice = price->getPrice();
}

void AgentCrossWealth::preStepUpdate(){

}
void AgentCrossWealth::postStepUpdate(){

}

