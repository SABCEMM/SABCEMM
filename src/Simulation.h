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
#include "Switching/ShareCalculator.h"
#include "StockExchange/StockExchange.h"
#include "RandomGenerator/RandomGenerator.h"
#include "ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "PriceCalculator/PriceCalculator.h"
#include "DataCollector/DataCollector.h"
#include "Writer/Writer.h"
#include "Input/Input.h"
#include "VariableContainer/VariableContainer.h"
#include "QuantitiesOfInterest/QuantitiesOfInterest.h"

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#include "QuantitiesOfInterest/QuantitiesOfInterest.h"

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
FRIEND_TEST(fullSimulationTest, fullSimulation_DCA_HPM);
FRIEND_TEST(fullSimulationTest, fullSimulation_TPA_W);
FRIEND_TEST(fullSimulationTest, fullSimulation_TPAC_W);
FRIEND_TEST(fullSimulationTest, fullSimulation_RII);
FRIEND_TEST(fullSimulationTest, fullSimulation_EMB);
FRIEND_TEST(GroupTest, dynamicGroups);
#endif

private:
	Input input;
	StockExchange* stockExchange; /**< Pointer to a StockExchange */
	RandomGenerator* randomNumberPool;
	DataCollector* dataCollector; /**< Pointer to a DataCollector composite */
	ExcessDemandCalculator* excessDemandCalculator; /**< Pointer to an ExcessDemandCalculator */
	PriceCalculator* priceCalculator; /**< Pointer to a PriceCalculator */
	std::vector<Agent*>* agents; /**< Vector of pointers to Agent */
	std::size_t numSteps; /**< Number of steps the simulation will run*/
    ShareCalculator* shareCalculator;
    std::vector<Switchable*> switchables;
	VariableContainer* variableContainer;
	QuantitiesOfInterest* quantitiesOfInterest;
	double simulationTime;


    void parse(Input& input);
    void preSimulation();
    /// automatically called in destructor.
    void postSimulation();
    ///
    /// \brief runSimulation
    /// \param[in, out] aborted Returns immediately after the completion of current timestep if aborted is true.
    /// \return true on success, false on errors
    ///
	bool runSimulation(bool &aborted, size_t id);

public:
	Simulation();

    ~Simulation();

    static bool executeSimulations(Input::fromFile inputs, bool &aborted);
	static void write(std::vector<Simulation*>* simulations, Writer* writer);

	typedef std::chrono::high_resolution_clock Clock;

	void write(Writer* writer);
};

#endif
