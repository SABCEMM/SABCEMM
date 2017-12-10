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

#ifndef _StockExchange_hpp_
#define _StockExchange_hpp_

#include <cstddef> //for std::size_t
#include <vector>
#include "../Parameter/Parameter.h"
#include "../Agent/Agent.h"
#include "../DataCollector/DataCollector.h"
#include "../RandomGenerator/RandomGenerator.h"
#include "../PriceCalculator/PriceCalculator.h"
#include "../ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "../VariableContainer/Dividend.h"
#include "../VariableContainer/GlobalNews.h"

/** This class represents the stock exchange.
 * The class is responsible for the iteration process. In each step a new price is computed, the agents buy/sell and a new excess demand is computed
 */
class StockExchange {
#if BUILD_TESTS
    FRIEND_TEST(stockExchange, preStep);
    FRIEND_TEST(stockExchange, step);
    FRIEND_TEST(fullSimulationTest, fullSimulation_Harras);
    FRIEND_TEST(fullSimulationTest, fullSimulation_Cross);
    FRIEND_TEST(fullSimulationTest, fullSimulation_FW);
#endif
protected:
	std::size_t numAgents; /**< Amount of agents in the agents vector. Stored seperatly to avoid calling agents->size() so often. */
	std::vector<Agent*>* agents; /**< Pointer to the vector of Agent */
	DataCollector* dataCollector; /**< Pointer to the DataCollector composite */
	RandomGenerator* randomGenerator; /**< Pointer to the randomGenerator */
	PriceCalculator* priceCalculator; /**< Pointer to the priceCalculator */
	ExcessDemandCalculator* excessDemandCalculator; /**< Pointer to the excessDemandCalculator */
	Dividend* dividend;
	vector<std::size_t> agentIndex; /**< Index of all agents. Can be shuffeld for the random update. */
	GlobalNews* globalNews; /**< Pointer to the GlobalNews container */

	StockExchange();
	StockExchange(DataCollector* newDataCollector, std::vector<Agent*>* newAgents, RandomGenerator* newRandomGenerator,
				  PriceCalculator* newPriceCalculator, ExcessDemandCalculator* newExcessDemandCalculator,
				  Dividend* newDividend, GlobalNews* newGlobalNews);

public:

	static StockExchange* factory();
	static StockExchange* factory(DataCollector* newDataCollector, std::vector<Agent*>* newAgents,
										  RandomGenerator* newRandomGenerator, PriceCalculator* newPriceCalculator,
										  ExcessDemandCalculator* newExcessDemandCalculator, Dividend* newDividend,
										  GlobalNews* newGlobalNews);

	void setDataCollector(DataCollector* newDataCollector);

    virtual void setAgents(std::vector<Agent*>* newAgents);
	void setRandomGenerator(RandomGenerator* newRandomGenerator);
	void setPriceCalculator(PriceCalculator* newPriceCalculator);
	void setExcessDemandCalculator(ExcessDemandCalculator* newExcessDemandCalculator);
    void setDividend(Dividend* newDividend);
	void setNumAgents();
	void setGlobalNews(GlobalNews* newGlobalNews);
	void checkInitilisation();

	virtual void preStep();
	virtual void step();
	virtual void postStep();

	virtual ~StockExchange();
};

#endif
