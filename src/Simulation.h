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

#ifndef _Simulation_hpp_
#define _Simulation_hpp_

#include <vector>
#include <cstddef> //for std::size_t
#include "StockExchange/StockExchange.h"
#include "RandomGenerator/RandomGenerator.h"
#include "ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "PriceCalculator/PriceCalculator.h"
#include "DataCollector/DataCollector.h"
#include "Writer/Writer.h"
#include "Parameter/Parameter.h"
#include "VariableContainer/VariableContainer.h"

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

#include <chrono>

/** Class to start the simulation.
 * This class separates the user interface from the numerical computation.
 * The class is responsible for the construction and initialization of the
 * StockExchange, RandomGenerator, DataCollector, PriceCalculator,Analysis and Agent.
 */
class Simulation {
#if BUILD_TESTS
FRIEND_TEST(fullSimulationTest, fullSimulation_Harras);
FRIEND_TEST(fullSimulationTest, fullSimulation_Cross);
FRIEND_TEST(fullSimulationTest, fullSimulation_FW);
#endif

private:
	Parameter* parameter;
	StockExchange* stockExchange; /**< Pointer to a StockExchange */
	RandomGenerator* randomNumberPool;
	DataCollector* dataCollector; /**< Pointer to a DataCollector composite */
	ExcessDemandCalculator* excessDemandCalculator; /**< Pointer to an ExcessDemandCalculator */
	PriceCalculator* priceCalculator; /**< Pointer to a PriceCalculator */
	Writer* writer; /**< Pointer to a Writer */
	std::vector<Agent*>* agents; /**< Vector of pointers to Agent */
	std::size_t numSteps; /**< Number of steps the simulation will run*/
	VariableContainer* variableContainer;


    void parse(Parameter* parameter);
    void preSimulation();
    void postSimulation();
    void runSimulation();

public:
	Simulation();

	~Simulation();

	static bool executeSimulations(vector<Parameter*> newSimulations);
    static bool executeSimulations(Parameter* newSimulations);

	typedef std::chrono::high_resolution_clock Clock;
};

#endif
