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
 * @file AgentHarras.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <vector>
#include <cmath>
#include <cassert>
#include <cstddef> //for std::size_t
#include "Agent.h"
#include "AgentHarras.h"


/** Updates the opinion of the AgentHarras. Combines the neighbourhoodInformation, globalNews and the
 * personalInformation
 * to the opinion. The three sources of information are multiplied with the respective trust coefficient and the
 * "Personality" coefficients of the agent.
 */
void AgentHarras::updateOpinion() {

	assert(randomGenerator != nullptr);
	assert(globalNews != nullptr);

	randomGenerator->getNormalRandomDouble(0, 1, &personalInformation);
    opinion = c1 * neighbourhoodInformation / static_cast<double>(neighbours.size()) + c2 * oldU * globalNews->getCurGlobalNews()
			  + c3 * personalInformation;

}


/** Update the neighbourhoodInformation of the Agent. Also recalculate the trust k[i] in the respective neighbour.
 */
void AgentHarras::updateNeighbourhoodInformation() {

	assert(excessDemand != nullptr);
	assert(countNeighbours == neighbours.size());
	assert(countNeighbours == decisionNeighbours.size());
	assert(countNeighbours == k.size());

	neighbourhoodInformation = 0;

    // moved here from updateU as the calculation of k needs an up-to-date sigma, which in turn needs an up-to-date kr.

    //Ohne DeltaT Skalierung
    //kr = alpha * kr + (1-alpha) * prevExcessDemand;// ==> <r(t)>
    kr = kr + (alpha-1) * deltaT->getDeltaT() * kr + (1-alpha) * deltaT->getDeltaT() * prevExcessDemand;
    //Ohne DeltaT Skalierung
    //sigma = sqrt(alpha * pow(sigma,2) + (1-alpha) * pow(prevExcessDemand-kr,2));
    sigma = sqrt(pow(sigma,2) + (alpha-1) * deltaT->getDeltaT() * pow(sigma,2) + (1-alpha) * deltaT->getDeltaT() *
																						 pow(prevExcessDemand-kr,2));

    // intel compiler needs explicit prefixing
    if(std::isnan(sigma))
    {

        sigma = 1e-11;
    }


	for (std::vector<Agent*>::size_type i=0; i<countNeighbours; i++) {
		//Ohne DeltaT Skalierung
		//k[i] = alpha * k[i] + (1.0 - alpha) * decisionNeighbours[i] * excessDemand->getExcessDemand() / sigma;

        double oldK = k[i];
        k[i] = k[i] + (alpha-1) * deltaT->getDeltaT() * k[i] + (1.0 - alpha) * deltaT->getDeltaT() *
																	   decisionNeighbours[i] *
																	   excessDemand->getExcessDemand() /
																	   (sigma*marketDepth);

        decisionNeighbours[i] = neighbours[i]->getDecision();
        // the opinion will be updated from kij(t-1), therefore oldK must be used here.
        neighbourhoodInformation += oldK * decisionNeighbours[i];
	}

}


/** Getter method for the current trust in the neighbours.
 * Intended for logging with the DataCollectorK only!
 */
const std::vector<double>& AgentHarras::getK() const{


	return k;
}


/** Setter for the neighbours of the agent. Also reinitializes the trust and the decisions in the neighbours randomly.
 * \param newNeighbours
 */
void AgentHarras::setNeighbours(const std::vector<Agent *> &newNeighbours) {

	assert(randomGenerator != nullptr);

	neighbours = newNeighbours;
	countNeighbours = neighbours.size();
	k.clear();
	decisionNeighbours.clear();
	randomGenerator->getUniformRandomInt(-1, 1, &decisionNeighbours, countNeighbours);
	randomGenerator->getUniformRandomDouble(0, 1, &k, countNeighbours);

}



/** Adds a neighbour to the agent. Initializes the trust and the neighbours decision randomly.
 * \param newNeighbour The new neighbour of the agent.
 */
void AgentHarras::addNeighbour(Agent* newNeighbour){

	assert(randomGenerator != nullptr);

	int tempDecision;
	double tempK;
	randomGenerator->getUniformRandomInt(-1,1,&tempDecision);
	randomGenerator->getUniformRandomDouble(0,1,&tempK);

	neighbours.push_back(newNeighbour);
	countNeighbours = neighbours.size();
	k.push_back(tempK);
	decisionNeighbours.push_back(tempDecision);

	assert(k.size() == countNeighbours);
	assert(decisionNeighbours.size() == countNeighbours);
	assert(countNeighbours == neighbours.size());


}


/** Forget the neighbourhood and clear trust (k) and decisions of the old neighbours.
 */
void AgentHarras::clearNeighbours(){

	neighbours.clear();
	k.clear();
	countNeighbours = 0;
	decisionNeighbours.clear();

}


/** Update the current cash of the agent. Depending on the new price and the old decision/tradingVolume.
 */
void AgentHarras::updateCash() {

	//cash = cash - decision * tradingVolume * price->getPrice();
	cash = cash - decision * tradingVolume * price->getPrice() * deltaT->getDeltaT();


}


/** Update the current stock of the agent. Depending on the old decision and tradingVolume.
 */
void AgentHarras::updateStock(){

	//stock = stock + decision * tradingVolume;
	stock = stock + decision * tradingVolume * deltaT->getDeltaT();

}


/** Updates the decision and the tradingVolume of the agent. Depends on the opinion of the agent.
 * If the opinion is greater then the agent's threshold, he buys stock --> decision +1
 * If the opinion is smaller then minus the agent's threshold, he sells stock --> decision -1
 * In any other case the agent remains passive --> decision 0
 * The agent is only allowed to buy/sell with the percentage g of his cash/stock.
 */
void AgentHarras::updateTradingVolume(const double& newPrice){

	if(opinion > threshold)
	{
		decision = 1;
		tradingVolume = g * cash / newPrice;
		if(tradingVolume<0) //Prohibit buying with debt
			{
				tradingVolume = 0;
			}
	}
	else if(opinion < -threshold)
	{
		decision = -1;
		tradingVolume = g * stock;
		if(tradingVolume < 0) //Prohibit buying with negative amount of stock
		{
			tradingVolume = 0;
		}
	}
	else
	{
		decision = 0;
		tradingVolume = 0;
	}

}

void AgentHarras::updateBisection(const double& newIterPrice){
	updateTradingVolume(newIterPrice);
}

/** Standard constructor of AgentHarras
 */
AgentHarras::AgentHarras():
		AgentHarras(nullptr, nullptr, nullptr, nullptr, 0, 0){


}


/** Constructor of AgentHarras. Requires a randomGenerator, a Price container,
 *  an ExcessDemand container and a GlobalNews container to work.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newExcessDemand Pointer to the ExcessDemand container
 *  \param newGlobalNews Pointer to the GlobalNews container
 */
AgentHarras::AgentHarras(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand,
						 GlobalNews* newGlobalNews):
		AgentHarras(newRandomGenerator, newPrice, newExcessDemand, newGlobalNews, 0, 0){


}


/** Constructor of AgentHarras. Requires a randomGenerator, a Price container, an ExcessDemand container
 *  and a GlobalNews container to work. Also sets the initial cash and stock of the agent.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newExcessDemand Pointer to the ExcessDemand container
 *  \param newGlobalNews Pointer to the GlobalNews container
 *  \param newCash Initial cash of the agent
 *  \param newStock Initial stock of the agent
 */
AgentHarras::AgentHarras(RandomGenerator* newRandomGenerator, Price* newPrice,
						 ExcessDemand* newExcessDemand, GlobalNews* newGlobalNews,
						 double newCash, double newStock):
		AgentHarras(newRandomGenerator, newPrice,
					newExcessDemand, newGlobalNews,
					newCash, newStock,
					0, 0, 0,
					0, 0,
					0, 0)
		{

}

AgentHarras::AgentHarras(RandomGenerator* newRandomGenerator, Price* newPrice,
						 ExcessDemand* newExcessDemand, GlobalNews* newGlobalNews,
						 double newCash, double newStock,
						 double C1, double C2, double C3,
						 double Threshold, double newG,
						 double newAlpha, double marketDepth):
		Agent(newRandomGenerator, newPrice, newCash, newStock),
        alpha(newAlpha),
        g(newG),
        excessDemand(newExcessDemand),
        globalNews(newGlobalNews),
		marketDepth(marketDepth)
{

	opinion=0;

	u=0;
	oldU=0;

	countNeighbours=0;
	neighbours.clear();
	k.clear();
	personalInformation=0;
	neighbourhoodInformation=0;
	decisionNeighbours.clear();
	kr=0;
	prevExcessDemand = 0;

    assert(randomGenerator != nullptr);
	randomGenerator->getUniformRandomDouble(0, C1, &c1);
	randomGenerator->getUniformRandomDouble(0, C2, &c2);
	randomGenerator->getUniformRandomDouble(0, C3, &c3);
	randomGenerator->getUniformRandomDouble(0, Threshold, &threshold);


	sigma = 0.1;
	decision = 0;

	// the following line might be correct for actual simulations.
	// randomGenerator->getUniformRandomDouble(0, 1, &personalInformation);
	kr = 0;
	// debugging the matlab script yields that for the first iteration,
	// prevExcessDemand is zero (compare obj.updateWeights in the script).
	prevExcessDemand = 0;

	// iAgent.m:87 (note that the matlab script stores u for each time step,
	//   therefore u is a vector there.)


	tradingVolume = 0;
}




/** Updates the trust u in the globalNews.
 */
void AgentHarras::updateU(){

	//Update kr, sigma and u (Trust in global News)

	//Ohne DeltaT Skalierung
	//u = alpha * u + (1.0 - alpha) * globalNews->getPrevGlobalNews() * excessDemand->getExcessDemand() / sigma;
    oldU = u;
    u = oldU + (alpha-1) * deltaT->getDeltaT() * oldU + (1.0 - alpha) * sqrt(deltaT->getDeltaT()) *
																globalNews->getPrevGlobalNews() *
																excessDemand->getExcessDemand() / (sigma*marketDepth);

}

/** Called every time step after the new Price was calculated to update the entire agent.
 */
void AgentHarras::stepUpdate(){

	assert(price!=nullptr);

    updateCash();
    updateStock();
	updateNeighbourhoodInformation();
	updateU();
	updateOpinion();
	updateTradingVolume(price->getPrice());
	prevExcessDemand = excessDemand->getExcessDemand();

}

void AgentHarras::preStepUpdate(){

}
void AgentHarras::postStepUpdate(){

}


/** Getter method for the countNeighbours variable.
 */
std::size_t AgentHarras::getCountNeighbours() const{


	return countNeighbours;
}
