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

#include "gtest/gtest.h"

#include "../../src/ExcessDemandCalculator/ExcessDemandCalculatorHarras.h"
#include "../DummyClasses/AgentDummy.h"
#include "../DummyClasses/RandomGeneratorFunctional.h"
#include <cstddef> //for std::size_t

TEST(ExcessDemandCalculatorHarras, stepCalculate){
	std::vector<Agent*> newAgents;
	ExcessDemand excessDemand;

    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	for(int i = 0;i < 16; i++){
		AgentDummy* tempAgent = new AgentDummy(randomGenerator, nullptr, 0, 0);
		tempAgent->setTo(1);
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	ExcessDemandCalculatorHarras excessDemandCalculator(&newAgents, &excessDemand);
	excessDemandCalculator.stepCalculate();

	EXPECT_EQ(excessDemand.getExcessDemand(), 1);

	for(std::size_t i = 0;i < 16; i++){
		delete newAgents.at(i);
	}
    delete randomGenerator;
}

TEST(ExcessDemandCalculatorHarras, preStepCalculate){
	std::vector<Agent*> newAgents;
	ExcessDemand excessDemand(7);

    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	for(int i = 0;i < 16; i++){
		AgentDummy* tempAgent = new AgentDummy(randomGenerator, nullptr, 0, 0);
		tempAgent->setTo(1);
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	ExcessDemandCalculatorHarras excessDemandCalculator(&newAgents, &excessDemand);
	excessDemandCalculator.preStepCalculate();

	EXPECT_EQ(excessDemand.getExcessDemand(), 7);

	for(std::size_t i = 0;i < 16; i++){
		delete newAgents.at(i);
	}
    delete randomGenerator;
}

TEST(ExcessDemandCalculatorHarras, postStepCalculate){
	std::vector<Agent*> newAgents;
	ExcessDemand excessDemand(8);

    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	for(int i = 0;i < 16; i++){
		AgentDummy* tempAgent = new AgentDummy(randomGenerator, nullptr, 0, 0);
		tempAgent->setTo(1);
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	ExcessDemandCalculatorHarras excessDemandCalculator(&newAgents, &excessDemand);
	excessDemandCalculator.postStepCalculate();

	EXPECT_EQ(excessDemand.getExcessDemand(), 8);

	for(std::size_t i = 0;i < 16; i++){
		delete newAgents.at(i);
	}
    delete randomGenerator;
}