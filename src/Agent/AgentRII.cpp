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
 * @file AgentRII.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */


#include "AgentRII.h"

#include "../Algorithms/brent/brent.hpp"





/** Standard constructor of AgentRII
 */
AgentRII::AgentRII():
		AgentRII(nullptr, nullptr, 0, 0){


}


/** Constructor of AgentRII. Requires a randomGenerator, a Price container,
 *  an ExcessDemand container and a GlobalNews container to work.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 */
AgentRII::AgentRII(RandomGenerator* newRandomGenerator, Price* newPrice):
		AgentRII(newRandomGenerator, newPrice, 0, 0){


}


/** Constructor of AgentRII. Requires a randomGenerator, a Price container, an ExcessDemand container
 *  and a GlobalNews container to work. Also sets the initial cash and stock of the agent.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newCash Initial cash of the agent
 *  \param newStock Initial stock of the agent
 */
AgentRII::AgentRII(RandomGenerator* newRandomGenerator, Price* newPrice,
				   double newCash, double newStock):
		AgentRII(newRandomGenerator, newPrice,
				 newCash, newStock,nullptr,
				 0,0,0,0){

}

AgentRII::AgentRII(RandomGenerator* newRandomGenerator, Price* newPrice,
				   double newCash, double newStock,Dividend* newDividend,
				   double newRiskTolerance, double newStdNoiseSigma,
				   double newInterestRate, double newK):
		Agent(newRandomGenerator, newPrice, newCash, newStock),
		riskTolerance (newRiskTolerance),
		stdNoiseSigma(newStdNoiseSigma),
		interestRate(newInterestRate),
		k(newK),
        dividend(newDividend)
{
	assert(randomGenerator != nullptr);
	randomGenerator->getUniformRandomDouble(0,1,&gamma);
	iterGamma = gamma;

	oldDividend = dividend->getDividend();
	oldPrice = price->getPrice();
}

/**
 */
void AgentRII::updateCash() {

	assert(deltaT != nullptr);
	assert(price != nullptr);
	assert(dividend != nullptr);

	double relPriceChange = (price->getPrice() - oldPrice + deltaT->getDeltaT() * dividend->getDividend())/
			(deltaT->getDeltaT() * price->getPrice());

	cash = cash + deltaT->getDeltaT() * (
			(1-gamma) * interestRate * cash
		  + gamma * cash * relPriceChange
			);

}


/**
 */
void AgentRII::updateTradingVolume(const double& newGamma, const double& newPrice){

	assert(price != nullptr);

	tradingVolume = newGamma * cash / newPrice;


}


/**
 */
void AgentRII::stepUpdate(){

	assert(price!=nullptr);
	assert(deltaT != nullptr);

	gamma = iterGamma;
	double noiseGamma = 0;
	randomGenerator->getNormalRandomDouble(0,stdNoiseSigma,&noiseGamma);
	gamma += noiseGamma;

	if(gamma > 1){
		gamma = 1;
	}
	if(gamma < 0){
		gamma = 0;
	}

	updateCash();

	updateBisection(price->getPrice());

	oldPrice = price->getPrice();
	oldDividend = dividend->getDividend();


}

void AgentRII::preStepUpdate(){

}
void AgentRII::postStepUpdate(){

}

/**
 */
void AgentRII::updateBisection(const double& newIterPrice){


	hypoCash = cash + deltaT->getDeltaT() * ((1-gamma) * interestRate * cash
				+ gamma * cash * (newIterPrice - price->getPrice() + deltaT->getDeltaT() * dividend->getDividend())
				  /(deltaT->getDeltaT() * newIterPrice));


	auto f = [this, newIterPrice](double x)->double { return this->expectedUtility(x, newIterPrice); };

	//Absch�tzung zweite Ableitung f''(x) <= m
	double m = 1e2; //TODO: Eigentlich 0 aber wir gehen mal sicher
	brent::glomin(0.01,0.99,0.0,m, 1e-4, 1e-4,f,iterGamma);

}




double AgentRII::expectedUtility(double newGamma, const double& iterPrice){
	assert(Util::doubleNotEqual(riskTolerance,1.0));
	double EU = 0;

	double a1 = (1-newGamma)*(1+interestRate * deltaT->getDeltaT()) +
			(newGamma / iterPrice) * (1 + dividend->getZ1() * deltaT->getDeltaT()) * oldDividend *
					(1 + deltaT->getDeltaT() * k)/(k-dividend->getExpectedValue());
	double a2 = (1-newGamma)*(1+interestRate * deltaT->getDeltaT()) +
			(newGamma / iterPrice) * (1 + dividend->getZ2() * deltaT->getDeltaT()) * oldDividend *
					(1 + deltaT->getDeltaT() * k)/(k-dividend->getExpectedValue());

	EU = pow(hypoCash , 1-riskTolerance) /
			((1-riskTolerance) * (dividend->getZ2() - dividend->getZ1()) * (2 - riskTolerance))
			* (iterPrice * ( k - dividend->getExpectedValue()) )
		 / ( newGamma * oldDividend * (1+deltaT->getDeltaT() * k))
			* (pow(a2,2-riskTolerance)-pow(a1,2-riskTolerance));

	return -EU; //returns -EU so that a minimizer can be used instead of a maximizer
}

/**void AgentRII::updateHypoWealth(){
	hypoWealth = (1-gamma) * hypoWealth * (1 + interestRate * deltaT->getDeltaT()) + gamma * hypoWealth *
 (1 + hypoRelativePrice * deltaT->getDeltaT());
}

void AgentRII::updateHypoRelativePrice(){
	double z = 0;
	randomGenerator->getUniformRandomDouble(dividend->getZ1(), dividend->getZ2(), &z);

	double a = ( 1 + dividend->getExpectedValue() * deltaT->getDeltaT()) / (k - dividend->getExpectedValue());

	hypoRelativePrice = ( oldDividend * ( 1 + z * deltaT->getDeltaT()) * (a + deltaT->getDeltaT()) - price->getPrice() )
 / (deltaT->getDeltaT() * price->getPrice());
}
**/
