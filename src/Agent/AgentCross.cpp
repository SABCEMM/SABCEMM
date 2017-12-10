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
 * @file AgentCross.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */


#include <cmath>
#include "Agent.h"
#include "AgentCross.h"


/** Standard constructor of AgentCross
 */
AgentCross::AgentCross():
		AgentCross(nullptr, nullptr, nullptr, 0, 0){


}


/** Constructor of AgentCross. Requires a randomGenerator, a Price container,
 *  an ExcessDemand container and a GlobalNews container to work.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newExcessDemand Pointer to the ExcessDemand container
 */
AgentCross::AgentCross(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand):
		AgentCross(newRandomGenerator, newPrice, newExcessDemand, 0, 0){


}


/** Constructor of AgentCross. Requires a randomGenerator, a Price container, an ExcessDemand container
 *  and a GlobalNews container to work. Also sets the initial cash and stock of the agent.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newExcessDemand Pointer to the ExcessDemand container
 *  \param newCash Initial cash of the agent
 *  \param newStock Initial stock of the agent
 */
AgentCross::AgentCross(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand,
		double newCash, double newStock):
		AgentCross(newRandomGenerator, newPrice, newExcessDemand,
				   newCash, newStock, nullptr,0, 0, 0, 0){


}

AgentCross::AgentCross(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand,
double newCash, double newStock, DeltaT* deltaT, double b1, double b2, double A1, double A2):
Agent(newRandomGenerator, newPrice, newCash, newStock){

	excessDemand = newExcessDemand;

	inactionLowerBound=0;
	inactionUpperBound=0;
	herdingPressure=0;

	inactionThreshold=0;
	herdingThreshold=0;

	tradingVolume = 1;

	assert(deltaT != nullptr);
	assert(price != nullptr);
	assert(randomGenerator != nullptr);
	this->deltaT = deltaT;

	double B1 = b1 * deltaT->getDeltaT();
	double B2 = b2 * deltaT->getDeltaT();

	randomGenerator->getUniformRandomDouble(A1, A2, &inactionThreshold);
	randomGenerator->getUniformRandomDouble(B1, B2, &herdingThreshold);

	randomGenerator->getUniformRandomDouble(B1, B2, &herdingPressure); // Init so sinnvoll? TODO

	double tempDecision;
	randomGenerator->getUniformRandomDouble(-1,1,&tempDecision);

	if(tempDecision > 0){
		decision = 1;
	}
	else{
		decision = -1;
	}

	updateBounds(price->getPrice());
}


/**
 */
void AgentCross::updateBounds(double price)
{
    inactionLowerBound = price / (1 + inactionThreshold);
    inactionUpperBound = price * (1 + inactionThreshold);
}

void AgentCross::stepUpdate(){


	if(decision * excessDemand->getExcessDemand() < 0){
		herdingPressure = herdingPressure + deltaT->getDeltaT() * fabs(excessDemand->getExcessDemand());
	}

	if (herdingPressure > herdingThreshold || price->getPrice() < inactionLowerBound ||
			price->getPrice() > inactionUpperBound) {
		decision = -decision;
        updateBounds(price->getPrice());
        herdingPressure = 0; /// @todo Laut Paper reset zu 0. Aus randomGenerator sinnvoller?
	}


}

void AgentCross::preStepUpdate(){

}
void AgentCross::postStepUpdate(){

}


/**
 */
void AgentCross::updateBisection(const double& newIterPrice){


		if (newIterPrice < inactionLowerBound || newIterPrice > inactionUpperBound) {
			decision = -decision;
			// @TODO: Update Bounds hier sinnvoll? Eigentlich sollten die Bounds doch beim Iterieren gleich bleiben ...
            //updateBounds(newIterPrice);
		}


}


