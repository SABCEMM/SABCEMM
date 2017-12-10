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
 * @file Agent.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "Agent.h"
#include "AgentCross.h"
#include "AgentHarras.h"
#include "AgentCrossWealth.h"
#include "AgentEMB.h"
#include "AgentFW.h"
#include "AgentFWFundamentalist.h"
#include "AgentFWChartist.h"

#include "../RandomGenerator/RandomGenerator.h"
#include "../VariableContainer/DeltaT.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/GlobalNews.h"
#include "../VariableContainer/Dividend.h"

#include "../Parameter/Parameter.h"
#include "../Parameter/ParameterSetAgent.h"
#include "../NeighbourhoodGenerator/LatticeNeighbourhoodGenerator.h"


#include <string>
#include <vector>

/** Standard constructor for the Agent.
 */
Agent::Agent(): Agent(nullptr, nullptr, 0, 0){


}

/** Constructor for the pure-virtual Agent.
 * Requires a RandomGenerator and the Price Container.
 * \param newRandomGenerator Pointer on the RandomGenerator
 * \param newPrice Pointer to the Price container
 */
Agent::Agent(RandomGenerator* newRandomGenerator, Price* newPrice): Agent(newRandomGenerator, newPrice, 0, 0){


}

/** Extended constructor for the pure-virtual Agent.
 * Requires a RandomGenerator  and the Price Container.. Initializes the cash and the stock of
 * an agent to user-defined values.
 * \param newRandomGenerator Pointer on the RandomGenerator
 * \param newPrice Pointer to the Price container
 * \param newCash Amount of cash
 * \param newStock Amount of stock
 */
Agent::Agent(RandomGenerator* newRandomGenerator, Price* newPrice, double newCash, double newStock){

	cash = newCash;
	stock = newStock;
	decision = 0;
	tradingVolume = 0;
	randomGenerator = newRandomGenerator;
	price = newPrice;
	deltaT = nullptr;

}


std::vector<Agent*>* Agent::factory(Parameter* parameter, RandomGenerator* randomNumberPool, Price* price,
									ExcessDemand* excessDemand, GlobalNews* globalNews, Dividend* dividend,
									DeltaT* deltaT){
	std::vector<Agent*>* agents = new std::vector<Agent*>;

	std::vector<Agent*> tempHarrasAgents;

	//Create Agents by Iterating over AgentClasses


	for (auto parameterSetAgent : parameter->parameterSetAgent) {
		tempHarrasAgents.clear();

		// Make Code more readable
		string type = *parameterSetAgent.type;


		for (std::size_t j = 0; j < *(parameterSetAgent.count); j++) {
			if (type == "AgentHarras") {
				auto * tempAgent = new AgentHarras(randomNumberPool, price, excessDemand, globalNews,
                                                   *(parameterSetAgent.cash), *(parameterSetAgent.stock),
                                                   *(parameterSetAgent.C1), *(parameterSetAgent.C2),
                                                   *(parameterSetAgent.C3), *(parameterSetAgent.threshold),
                                                   *(parameterSetAgent.g), *(parameterSetAgent.alpha),
                                                   *(parameter->parameterSetPriceCalculator.marketDepth));

				if(parameterSetAgent.groups){
					tempAgent->setGroups(*(parameterSetAgent.groups));
				}
				tempHarrasAgents.push_back(static_cast<Agent*>(tempAgent));
			}
			else if (type == "AgentCross") {
				auto * tempAgent = new AgentCross(randomNumberPool, price,	excessDemand,
                                                  *(parameterSetAgent.cash), *(parameterSetAgent.stock),
                                                  deltaT,
                                                  *(parameterSetAgent.b1), *(parameterSetAgent.b2),
                                                  *(parameterSetAgent.A1), *(parameterSetAgent.A2));
				if(parameterSetAgent.groups){
					tempAgent->setGroups(*(parameterSetAgent.groups));
				}
				agents->push_back(static_cast<Agent*>(tempAgent));
			}
			else if (type == "AgentCrossWealth") {
				auto * tempAgent = new AgentCrossWealth(randomNumberPool,
														price,
														excessDemand,
														*(parameterSetAgent.cash),
														*(parameterSetAgent.stock),
                										deltaT,
                                                        *(parameterSetAgent.b1), *(parameterSetAgent.b2),
                                                        *(parameterSetAgent.A1), *(parameterSetAgent.A2),
                                                        *(parameterSetAgent.gamma),
                                                        *(parameter->parameterSetDividend.interestRate));

				if(parameterSetAgent.groups){
					tempAgent->setGroups(*(parameterSetAgent.groups));
				}
                agents->push_back(static_cast<Agent*>(tempAgent));
            }
            else if (type == "AgentEMB") {
                auto * tempAgent = new AgentEMB(randomNumberPool, price,
                                                *(parameterSetAgent.cash), *(parameterSetAgent.stock),
                                                dividend, *(parameterSetAgent.riskTolerance),
                                                static_cast<std::size_t>(*(parameterSetAgent.memorySpan)),
                                                *(parameterSetAgent.stdNoiseSigma),
                                                *(parameter->parameterSetDividend.interestRate),
                                                *(parameterSetAgent.historyMean),
                                                *(parameterSetAgent.historySigma), *(parameterSetAgent.initialGamma));

                if(parameterSetAgent.groups){
                    tempAgent->setGroups(*(parameterSetAgent.groups));
				}

				agents->push_back(static_cast<Agent*>(tempAgent));
			}
			else if(type == "AgentFW")
			{
				// this is one of the rare cases where we actually rely on default values for null-optionals.
				// The reasoning here is: We assume that the Parameter object validated that for the chosen index
				// strategy, the correct variables are given. Otherwise, the program would have been aborted.
				// For certain index strategies, not all coefficients are used - they are therefore not specified in the
				// input file. Still, AgentFW expects these variables to be given. As they are not used, 0 is chosen
				// but any value would be fine.
				// We should NOT use this method in the Parameter object (therefore initializing all alphas with 0 here)
				// as we cannot warn the user about unspecified variables in the input file then.
				// The same reasoning applies to nu (which is only needed for TPA) and beta
				// (which is only needed for DCA)
				auto a1 = new AgentFWFundamentalist(
						randomNumberPool,
						price,
						*parameterSetAgent.eta,
						parameterSetAgent.beta.value_or(0),
						parameterSetAgent.alpha_w.value_or(0),
						parameterSetAgent.alpha_n.value_or(0),
						parameterSetAgent.alpha_p.value_or(0),
						parameterSetAgent.alpha_0.value_or(0),
						parameterSetAgent.nu.value_or(0),
						switchingStrategyToString.at(*parameterSetAgent.switchingStrategy),
						*parameterSetAgent.indexStrategy,
						*parameterSetAgent.sigma_f,
						*parameterSetAgent.phi,
						*parameterSetAgent.fundamentalPrice
				);
				auto a2 = new AgentFWChartist(
						randomNumberPool,
						price,
						*parameterSetAgent.eta,
						parameterSetAgent.beta.value_or(0),
						parameterSetAgent.alpha_w.value_or(0),
						parameterSetAgent.alpha_n.value_or(0),
						parameterSetAgent.alpha_p.value_or(0),
						parameterSetAgent.alpha_0.value_or(0),
						parameterSetAgent.nu.value_or(0),
						switchingStrategyToString.at(*parameterSetAgent.switchingStrategy),
						*parameterSetAgent.indexStrategy,
						*parameterSetAgent.sigma_c,
						*parameterSetAgent.chi
				);

				if(parameterSetAgent.groups) {
					a1->setGroups(*parameterSetAgent.groups);
					a2->setGroups(*parameterSetAgent.groups);
				}
				a1->setPartner(a2);
				a2->setPartner(a1);
				agents->push_back(dynamic_cast<Agent*>(a1));
				agents->push_back(dynamic_cast<Agent*>(a2));
			}

			else {
				throw("AgentClass unknown!");
			}
		}

		//If HarrasAgents are used create NeighbourhoodGenerator and create Neighbourhood

		if (type == "AgentHarras"){
			NeighbourhoodGenerator* neighbourhoodGenerator; /**< Pointer to a NeighbourhoodGenerator */
			if (*(parameterSetAgent.neighbourhoodGeneratorClass)
				== "LatticeNeighbourhoodGenerator") {
				neighbourhoodGenerator = new LatticeNeighbourhoodGenerator(&tempHarrasAgents);
			} else {
				throw("neighbourhoodGeneratorClass unknown!");
			}

			neighbourhoodGenerator->generateNeighbourhood();

			agents->insert(agents->end(), tempHarrasAgents.begin(), tempHarrasAgents.end());
			delete neighbourhoodGenerator;
		}
	}

	for (auto &agent : *agents) {
		agent->setDeltaT(deltaT);
	}

	return agents;
}

/** Virtual destructor of the Agent.
 */
Agent::~Agent() = default;

/** Getter method for the agent's current cash.
 * \return Agent's current cash
 */
double Agent::getCash() const{


	return cash;
}

/** Setter method for the agent's current cash.
 * \param newCash New amount of cash for the agent
 */
void Agent::setCash(double& newCash) {

	cash = newCash;

}

/** Getter method for agent's current decision.
 * \return Agent's current decision
 */
const int& Agent::getDecision() const{


	return decision;
}

/** Setter method for the randomGenerator.
 */
void Agent::setRandomGenerator(RandomGenerator* newRandomGenerator) {

	randomGenerator = newRandomGenerator;

}

/** Setter method for the Price container
 * \param newPrice Pointer to the new Price container
 */
void Agent::setPrice(Price* newPrice){

	price = newPrice;

}

/** Getter method for the agent's current stock.
 */
double Agent::getStock() const{


	return stock;
}

/** Setter methods for the agents stock
 * \param newStock
 */
void Agent::setStock(double& newStock){

	stock = newStock;

}

/** Getter method for the agent's current trading volume.
 */
const double& Agent::getTradingVolume() const{


	return tradingVolume;
}


void Agent::setDeltaT(DeltaT* newDeltaT){
	deltaT = newDeltaT;
}
