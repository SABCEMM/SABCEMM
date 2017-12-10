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
#include "math.h"



#include "../../src/PriceCalculator/PriceCalculatorBisection.h"
#include "../DummyClasses/AgentDummy.h"
#include "../../src/ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "../../src/ExcessDemandCalculator/ExcessDemandCalculatorHarras.h"
#include "../../src/VariableContainer/Price.h"
#include "../../src/VariableContainer/ExcessDemand.h"
#include "../DummyClasses/RandomGeneratorFunctional.h"
#include <cstddef> //for std::size_t

TEST(PriceCalculatorBisection, getExcessDemandAtPrice){
	DeltaT deltaT(1);
	double marketDepth = 1;
	Price price;
	ExcessDemand excessDemand(5);
	std::vector<Agent*> newAgents;

	RandomGenerator* randomGenerator;
	randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	for(int i = 0;i < 16; i++){
		AgentDummy* tempAgent = new AgentDummy(randomGenerator, &price);
		tempAgent->setTo(1);
		if(i%2 == 0){
			tempAgent->opinion = -5;
		}
		else{
			tempAgent->opinion = 5;
		}
		tempAgent->setDeltaT(&deltaT);
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}
	
	ExcessDemandCalculator* excessDemandCalculator;
	excessDemandCalculator = new ExcessDemandCalculatorHarras(&newAgents, &excessDemand);
	

	PriceCalculatorBisection priceCalculator(excessDemandCalculator, &price, &excessDemand);
	priceCalculator.setAgents(&newAgents);
	priceCalculator.setDeltaT(&deltaT);
	priceCalculator.setMarketDepth(marketDepth);
	

	double EDatPrice = priceCalculator.getExcessDemandAtPrice(1);
	
	EXPECT_EQ(EDatPrice, 0);

	EDatPrice = priceCalculator.getExcessDemandAtPrice(2);
	
	EXPECT_NE(EDatPrice, 0);

	EDatPrice = priceCalculator.getExcessDemandAtPrice(0);
	
	EXPECT_NE(EDatPrice, 0);

	delete excessDemandCalculator;
	for(std::size_t i = 0;i < 16; i++){
		delete newAgents.at(i);
	}

	delete randomGenerator;
}


TEST(PriceCalculatorBisection, stepCalculate){

	DeltaT deltaT(1);
	double marketDepth = 1;
	Price price;
	ExcessDemand excessDemand(5);
	std::vector<Agent*> newAgents;

    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	for(int i = 0;i < 16; i++){
		AgentDummy* tempAgent = new AgentDummy(randomGenerator, &price);
		tempAgent->setTo(1);
		if(i%2 == 0){
			tempAgent->opinion = -5;
		}
		else{
			tempAgent->opinion = 5;
		}
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	ExcessDemandCalculator* excessDemandCalculator;
	excessDemandCalculator = new ExcessDemandCalculatorHarras(&newAgents, &excessDemand);

	PriceCalculatorBisection priceCalculator(excessDemandCalculator, &price, &excessDemand);
	priceCalculator.setAgents(&newAgents);
	priceCalculator.setDeltaT(&deltaT);
	priceCalculator.setMarketDepth(marketDepth);

	priceCalculator.stepCalculate();
	EXPECT_LT(price.getPrice(), 1+priceCalculator.epsilon);
	EXPECT_GT(price.getPrice(), 1-priceCalculator.epsilon);

	EXPECT_LT(excessDemand.getExcessDemand(), priceCalculator.epsilon);
	EXPECT_GT(excessDemand.getExcessDemand(), -priceCalculator.epsilon);

	delete excessDemandCalculator;
	for(std::size_t i = 0;i < 16; i++){
		delete newAgents.at(i);
	}

    delete randomGenerator;
}

