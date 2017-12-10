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
#include <cassert>
#include <vector>
#include "StockExchange.h"
#include <cstddef> //for std::size_t
#include <algorithm>
#include <random>
#include <boost/core/ignore_unused.hpp>
#include <limits>


/** Setter method for the DataCollector composite
 * \param newDataCollector Pointer to the new DataCollector composite
 */
void StockExchange::setDataCollector(DataCollector* newDataCollector) {

	dataCollector = newDataCollector;

}


/** Setter method for the agents vector
 * \param newAgents Pointer to the new vector of agents.
 */
void StockExchange::setAgents(std::vector<Agent*>* newAgents){

	agents = newAgents;
	setNumAgents();

}


/** Setter method for the RandomGenerator
 * \param newRandomGenerator Pointer to a RandomGenerator
 */
void StockExchange::setRandomGenerator(RandomGenerator* newRandomGenerator) {

	randomGenerator = newRandomGenerator;

}


/** Setter method for the PriceCalculator
 * \param newPriceCalculator Pointer to the PriceCalculator
 */
void StockExchange::setPriceCalculator(PriceCalculator* newPriceCalculator) {

	priceCalculator = newPriceCalculator;

}


/** Setter method for the ExcessDemandCalculator
 * \param newExcessDemandCalculator Pointer to the ExcessDemandCalculator
 */
void StockExchange::setExcessDemandCalculator(ExcessDemandCalculator* newExcessDemandCalculator) {

	excessDemandCalculator = newExcessDemandCalculator;

}


/** Setter method for the number of agents. The number is not taken as a paramter but from the array of agents.
 */
void StockExchange::setNumAgents() {

	if (agents!=nullptr){
		numAgents = agents->size();
	}else{
		numAgents = 0;
	}

}


/** StandardConstructor of the StockExchange
 */
StockExchange::StockExchange(): StockExchange(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {


}


/** Constructor of the StockExchange.
 * \param newDataCollector Pointer to a DataCollector composite
 * \param newAgents Pointer to a vector of agents
 * \param newRandomGenerator Pointer to a RandomGenerator
 * \param newPriceCalculator Pointer to a PriceCalculator
 * \param newExcessDemandCalculator Pointer to an ExcessDemandCalculator
 */
StockExchange::StockExchange(DataCollector* newDataCollector, std::vector<Agent*>* newAgents,
                             RandomGenerator* newRandomGenerator, PriceCalculator* newPriceCalculator,
                             ExcessDemandCalculator* newExcessDemandCalculator, Dividend* newDividend,
							 GlobalNews* newGlobalNews){

	numAgents = 0;
	agents = newAgents;
	dataCollector = newDataCollector;
	randomGenerator = newRandomGenerator;
	priceCalculator = newPriceCalculator;
	excessDemandCalculator = newExcessDemandCalculator;
	dividend = newDividend;
	setNumAgents();
	agentIndex.clear();
	globalNews = newGlobalNews;
}


/** Destructor
 */
StockExchange::~StockExchange() = default;


/** Check if the object is fully configured and ready to be used.
 */
void StockExchange::checkInitilisation() {

	assert(agents != nullptr);
	assert(dataCollector != nullptr);
	assert(randomGenerator != nullptr);
	assert(priceCalculator != nullptr);
	assert(excessDemandCalculator != nullptr);
	assert(numAgents == agents->size());

}

void StockExchange::setDividend(Dividend* newDividend){
	assert(newDividend != nullptr);
	dividend = newDividend;
}

/** Setter method for the GlobalNews container
 * \param newGlobalNews Pointer to a GlobalNews container
 */
void StockExchange::setGlobalNews(GlobalNews* newGlobalNews){

	this->globalNews = newGlobalNews;

}


/** PreStep: Execute all calls that are required before a new update of the price and the agents can be triggered.
 * Generates new GlobalNews.
 */
void StockExchange::preStep() {

	assert(priceCalculator != nullptr);

	if(globalNews!=nullptr){
		globalNews->generateNewGlobalNews();
	}
	if(dividend!= nullptr){
		dividend->calculateDividend();
	}

	if(excessDemandCalculator != nullptr)
		excessDemandCalculator->preStepCalculate();

	priceCalculator->preStepCalculate();


	for (auto &agent : *agents) {
		agent->preStepUpdate();
	}

}

/** Step: Calculate a new Price and update all agents.
 * The agents are updated in a random order.
 */
void StockExchange::step(){

	assert(agents!=nullptr);
	assert(priceCalculator!=nullptr);
    assert(randomGenerator != nullptr);


	//Random over all Agents
	priceCalculator->stepCalculate();

	if(agentIndex.size() != agents->size()){
		agentIndex.clear();
		for(std::size_t j=0;j<agents->size();j++)
		{
			agentIndex.push_back(j);
		}
	}

    // shuffling needs to be coupled to the RNG to ensure reproducability for
    // equal seeds.
    int seed=0;
    randomGenerator->getUniformRandomInt(0, std::numeric_limits<int>::max(), &seed);
    std::mt19937 g(static_cast<unsigned int>(seed));
    std::shuffle(agentIndex.begin(),agentIndex.end(),g);

	for(std::size_t j=0;j<agents->size();j++)
	{
		agents->at(agentIndex[j])->stepUpdate();
	}


}


/** PostStep.
 * Calls the DataCollector.
 */
void StockExchange::postStep(){

	// ED-postInfo (the correction terms for the price) must be available
	//              before price correction.
	/// @todo this, however, is not general. Can we specify a rule that will
	///       likely hold for future models?
	if(excessDemandCalculator != nullptr)
		excessDemandCalculator->postStepCalculate();

	priceCalculator->postStepCalculate();

	for (auto &agent : *agents) {
		agent->postStepUpdate();
	}

	dataCollector->collect();
}

StockExchange* StockExchange::factory() {

    return new StockExchange();
}


StockExchange* StockExchange::factory(DataCollector* newDataCollector, std::vector<Agent*>* newAgents,
									  RandomGenerator* newRandomGenerator, PriceCalculator* newPriceCalculator,
									  ExcessDemandCalculator* newExcessDemandCalculator, Dividend* newDividend,
									  GlobalNews* newGlobalNews) {

	return new StockExchange(newDataCollector, newAgents,
							 newRandomGenerator, newPriceCalculator,
							 newExcessDemandCalculator, newDividend,
							 newGlobalNews);
}

