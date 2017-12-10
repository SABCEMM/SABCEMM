
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

#include "../../src/Agent/AgentCross.h"
#include "../DummyClasses/RandomGeneratorFunctional.h"



TEST(AgentCrossTest, constructor) {
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	DeltaT deltaT(1);
	Price price;
	ExcessDemand excessDemand;
	price.setPrice(1);

	double b1 = 1;
	double b2 = 1;
	double A1 = 1;
	double A2 = 1;



	AgentCross testAgent(randomGenerator, &price, &excessDemand,
			   3, 4, &deltaT,  b1,  b2,  A1,  A2);

	EXPECT_EQ(testAgent.cash, 3);
	EXPECT_EQ(testAgent.stock, 4);
	EXPECT_EQ(testAgent.deltaT, &deltaT);
	EXPECT_EQ(testAgent.randomGenerator, randomGenerator);
	EXPECT_EQ(testAgent.price, &price);
	EXPECT_EQ(testAgent.excessDemand, &excessDemand);
	EXPECT_EQ(testAgent.inactionThreshold, 1);
	EXPECT_EQ(testAgent.inactionThreshold, 1);
	EXPECT_EQ(testAgent.herdingPressure, 1);
	EXPECT_EQ(testAgent.herdingThreshold, 1);

	EXPECT_EQ(testAgent.decision, 1);

	EXPECT_EQ(testAgent.inactionLowerBound, 0.5);
	EXPECT_EQ(testAgent.inactionUpperBound, 2);

}

TEST(AgentCrossTest, stepUpdate) {

	DeltaT deltaT(1);
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	ExcessDemand excessDemand;
    double newExDem = 0.5;
	excessDemand.setExcessDemand(newExDem);
	Price price;
	price.setPrice(1);

	AgentCross testAgent(randomGenerator, &price, &excessDemand,
			  3, 4, &deltaT,  1,  1,  1,  1);

	testAgent.setDeltaT(&deltaT);

	testAgent.decision = -1;
    testAgent.herdingPressure = 0;

	testAgent.stepUpdate();

    EXPECT_EQ(testAgent.herdingPressure, 0.5);

    newExDem = 2;
    excessDemand.setExcessDemand(newExDem);
    testAgent.stepUpdate();

    EXPECT_EQ(testAgent.herdingPressure, 0);
	EXPECT_EQ(testAgent.decision, 1);
	EXPECT_EQ(testAgent.inactionLowerBound, 0.5);
	EXPECT_EQ(testAgent.inactionUpperBound, 2);

	delete randomGenerator;
}

TEST(AgentCrossTest, preStepUpdate) {

    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    Price price;

    AgentCross testAgent(randomGenerator, &price, nullptr,
                         3, 4, &deltaT,  1,  1,  1,  1);


    // Call function to test: We expect no change
    testAgent.preStepUpdate();

    EXPECT_EQ(testAgent.inactionLowerBound, 0.5);
    EXPECT_EQ(testAgent.inactionUpperBound, 2);
    EXPECT_EQ(testAgent.herdingPressure, 1);

    EXPECT_EQ(testAgent.inactionThreshold, 1);
    EXPECT_EQ(testAgent.herdingThreshold, 1);

    EXPECT_EQ(testAgent.tradingVolume, 1);

}

TEST(AgentCrossTest, postStepUpdate) {
    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    Price price;

    AgentCross testAgent(randomGenerator, &price, nullptr,
                         3, 4, &deltaT,  1,  1,  1,  1);

    // Call function to test: We expect no change
    testAgent.postStepUpdate();

    EXPECT_EQ(testAgent.inactionLowerBound, 0.5);
    EXPECT_EQ(testAgent.inactionUpperBound, 2);
    EXPECT_EQ(testAgent.herdingPressure, 1);

    EXPECT_EQ(testAgent.inactionThreshold, 1);
    EXPECT_EQ(testAgent.herdingThreshold, 1);

    EXPECT_EQ(testAgent.tradingVolume, 1);
}

// test the reaction to updated prices.
TEST(AgentCrossTest, updateBisection) {
	DeltaT deltaT(1);
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	Price price;
	price.setPrice(1);

	AgentCross testAgent(randomGenerator, &price, nullptr,
						 3, 4, &deltaT,  1,  1,  1,  1);

    testAgent.updateBisection(3);

	EXPECT_EQ(testAgent.decision, -1);
	// Bounds werden doch nicht upgedated
    //EXPECT_EQ(testAgent.inactionLowerBound, 3./(1+1));
    //EXPECT_EQ(testAgent.inactionUpperBound, 3*(1+1));
    EXPECT_EQ(testAgent.inactionLowerBound, 1./(1+1));
    EXPECT_EQ(testAgent.inactionUpperBound, 1*(1+1));

	delete randomGenerator;
}
