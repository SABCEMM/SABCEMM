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

#include <cmath>
#include <vector>

#include "../../src/Agent/AgentHarras.h"

#include "../../src/RandomGenerator/RandomGenerator.h"
#include "../../src/VariableContainer/GlobalNews.h"
#include "../../src/VariableContainer/ExcessDemand.h"
#include "../DummyClasses/RandomGeneratorFunctional.h"
#include <cstddef> //for std::size_t


TEST(AgentHarrasTest, updateOpinion) {
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	GlobalNews dummyGlobalNews(randomGenerator);

	AgentHarras testAgent(randomGenerator, nullptr, nullptr, &dummyGlobalNews);
    // add four dummy neighbours
    for(std::size_t i = 0; i < 4; ++i)
        testAgent.addNeighbour(NULL);

	testAgent.c1 = 1;
    testAgent.c2 = 2;
    testAgent.c3 = 3;
    testAgent.countNeighbours = 4;
    testAgent.neighbourhoodInformation = 5;
    testAgent.u = 6;
	
	testAgent.updateOpinion();

    // w = 1/#agents*C1*neighbourhoodInformation+C2*prevU*globalNews+C3*personalInfo
    // here: w = 1/4*5+2*0*1+3*1 = 4.25
    // (personalInformation is randomly drawn in updateOpinion, so it's 1 here)

    EXPECT_NEAR(testAgent.opinion, 4.25, 0.001);

	delete randomGenerator;
}

TEST(AgentHarrasTest, updateNeighbourhoodInformation) {
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	
	ExcessDemand excessDemand;
	excessDemand.excessDemand = 1;

	DeltaT deltaT(1);

	AgentHarras testAgent(randomGenerator, nullptr, &excessDemand, nullptr);
    testAgent.marketDepth = 0.5;

	//Nachbarn erstellen
	std::vector<Agent*> neighbours;
	for(int i=0;i<4;i++){
		AgentHarras* tempAgent;
		tempAgent = new AgentHarras(randomGenerator, nullptr, nullptr, nullptr);
		tempAgent->decision = 1;
		neighbours.push_back(tempAgent);
	}

	testAgent.setNeighbours(neighbours);
	testAgent.alpha = 0.5;
	testAgent.sigma = 1;
	testAgent.setDeltaT(&deltaT);

	testAgent.updateNeighbourhoodInformation();

    /// @todo choose prevExcessDemand such that kr is not zero and test kr here.

    // sigma = sqrt(pow(sigma,2) + (alpha-1) * deltaT->getDeltaT() * pow(sigma,2) + (1-alpha) * deltaT->getDeltaT() * pow(prevExcessDemand-kr,2));
    // here: sigma = sqrt(1-0.5*1*1+0.5*1*(0-0)^2) = sqrt(0.5)
    EXPECT_NEAR(testAgent.sigma, sqrt(0.5), 0.001);


	for(std::size_t i=0; i<4; i++){
        // k[i] = k[i] + (alpha-1) * deltaT->getDeltaT() * k[i] + (1.0 - alpha) * deltaT->getDeltaT() * decisionNeighbours[i] * excessDemand->getExcessDemand() / (sigma*marketDepth);
        // here: k[i] = 1+0-0.5*1*0+0.5*1*1*1/(sqrt(0.5)*0.5)= 0.5+1/sqrt(0.5)

        EXPECT_EQ(testAgent.decisionNeighbours[i],1);
        EXPECT_NEAR(testAgent.k[i],0.5+1/sqrt(0.5), 0.001);
	}

    EXPECT_NEAR(testAgent.neighbourhoodInformation,4*1, 0.001);

    /// @todo eine zweite Iteration durchrechnen.

	delete randomGenerator;
	for(unsigned long i=0; i < 4; i++){
		delete neighbours.at(i);
	}
}

TEST(AgentHarrasTest, updateCash) {
	Price price;
	DeltaT deltaT(1);

    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	AgentHarras testAgent(randomGenerator, &price, nullptr, nullptr);

	testAgent.decision = 1;
	testAgent.tradingVolume = 1;
	testAgent.cash = 1;
	testAgent.setDeltaT(&deltaT);	

	testAgent.updateCash(); 

	EXPECT_EQ(testAgent.cash,0);

    delete randomGenerator;
}

TEST(AgentHarrasTest, updateStock) {
	DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	AgentHarras testAgent(randomGenerator, nullptr, nullptr, nullptr);

	testAgent.decision = 1;
	testAgent.tradingVolume = 1;
	testAgent.stock = 1;
	testAgent.setDeltaT(&deltaT);

	testAgent.updateStock(); 

	EXPECT_EQ(testAgent.stock, 2);
    delete randomGenerator;
}

TEST(AgentHarrasTest, updateU) {
	DeltaT deltaT(1);
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	GlobalNews dummyGlobalNews(randomGenerator);
	ExcessDemand excessDemand;

	AgentHarras testAgent(randomGenerator, nullptr, &excessDemand, &dummyGlobalNews);

    testAgent.u = 1;
    testAgent.excessDemand->excessDemand = 3;
    testAgent.marketDepth = 0.5;
    testAgent.alpha = 0.5;
    testAgent.sigma = 2;
	testAgent.setDeltaT(&deltaT);

	testAgent.updateU();

    // oldU is set to u (which is 1) in testAgent.updateU
    // u = oldU + (alpha-1) * deltaT->getDeltaT() * oldU + (1.0 - alpha) * sqrt(deltaT->getDeltaT()) * globalNews->getPrevGlobalNews() * excessDemand->getExcessDemand() / (sigma*marketDepth);
    // here: u = 1 - 0.5*1*1 + 0.5*1*1*3/(2*0.5) = 2

    EXPECT_NEAR(testAgent.u, 2, 0.001);

	delete randomGenerator;
}

TEST(AgentHarrasTest, constructor) {
	RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

    Price price;
    ExcessDemand excessDemand;
    GlobalNews globalNews(randomGenerator);

	double C1 = 1;
    double C2 = 2;
    double C3 = 3;
    double Threshold = 4;
    double newG = 5;
    double newAlpha = 6;
    double newCash = 7;
    double newStock = 8;
    double marketDepth = 9;

	AgentHarras testAgent(randomGenerator, &price,
                          &excessDemand, &globalNews,
                          newCash, newStock,
                          C1, C2, C3,
                          Threshold, newG,
                          newAlpha, marketDepth);


	EXPECT_EQ(testAgent.c1, 1);
    /// @todo c1,c2,threshold are not properly tested due to the random generator
    /// (maybe introduce a RandomGenerator that draws numbers from a list?)
    EXPECT_EQ(testAgent.c2, 1);
    EXPECT_EQ(testAgent.c3, 1);
    EXPECT_EQ(testAgent.threshold, 1);
    EXPECT_EQ(testAgent.g, 5);
    EXPECT_EQ(testAgent.alpha, 6);
    EXPECT_EQ(testAgent.cash, 7);
    EXPECT_EQ(testAgent.stock, 8);
    EXPECT_EQ(testAgent.marketDepth, 9);
    EXPECT_EQ(testAgent.sigma, 0.1);
    /// @todo those were changed during #55 - verify that those initial values
    ///       are meaningful.
    EXPECT_EQ(testAgent.decision, 0);
    EXPECT_EQ(testAgent.personalInformation, 0);
    EXPECT_EQ(testAgent.prevExcessDemand, 0);
    EXPECT_EQ(testAgent.tradingVolume, 0);
    EXPECT_EQ(testAgent.u, 0);

    EXPECT_EQ(testAgent.randomGenerator, randomGenerator);
    EXPECT_EQ(testAgent.price, &price);
    EXPECT_EQ(testAgent.excessDemand, &excessDemand);
    EXPECT_EQ(testAgent.globalNews, &globalNews);

    delete randomGenerator;
}

TEST(AgentHarrasTest, stepUpdate) {

	DeltaT deltaT(1);
	RandomGenerator* randomGenerator;
	randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
	ExcessDemand excessDemand;
	GlobalNews globalNews(randomGenerator);
	Price price;
	AgentHarras testAgent(randomGenerator, &price, &excessDemand, &globalNews);

	testAgent.setDeltaT(&deltaT);

	testAgent.stepUpdate();
	EXPECT_EQ(testAgent.prevExcessDemand, excessDemand.getExcessDemand());
	delete randomGenerator;
}

TEST(AgentHarrasTest, preStepUpdate) {
    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    ExcessDemand excessDemand;
    GlobalNews globalNews(randomGenerator);
    Price price;
    AgentHarras testAgent(randomGenerator, &price, &excessDemand, &globalNews);

	// Call function to test: We expect no change
	testAgent.preStepUpdate();

    EXPECT_EQ( testAgent.c1, 1);
    EXPECT_EQ( testAgent.c2, 1);
    EXPECT_EQ( testAgent.c3, 1);
    EXPECT_EQ( testAgent.threshold, 1);
    EXPECT_EQ( testAgent.opinion, 0);
    EXPECT_EQ( testAgent.alpha, 0);
    EXPECT_EQ( testAgent.sigma, 0.1);
    EXPECT_EQ( testAgent.g, 0);
    EXPECT_EQ( testAgent.u, 0);
    EXPECT_EQ( testAgent.oldU, 0);
    EXPECT_EQ( testAgent.marketDepth, 0);
    EXPECT_EQ( testAgent.countNeighbours, 0);
    EXPECT_EQ( testAgent.personalInformation, 0);
    EXPECT_EQ( testAgent.neighbourhoodInformation, 0);
    EXPECT_EQ( testAgent.kr, 0);
    EXPECT_EQ( testAgent.prevExcessDemand ,  0);
    EXPECT_EQ( testAgent.marketDepth ,  0);

    delete randomGenerator;
}
TEST(AgentHarrasTest, postStepUpdate) {
    DeltaT deltaT(1);
    RandomGenerator* randomGenerator;
    randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));
    ExcessDemand excessDemand;
    GlobalNews globalNews(randomGenerator);
    Price price;
    AgentHarras testAgent(randomGenerator, &price, &excessDemand, &globalNews);

    // Call function to test: We expect no change
    testAgent.postStepUpdate();

    EXPECT_EQ( testAgent.c1, 1);
    EXPECT_EQ( testAgent.c2, 1);
    EXPECT_EQ( testAgent.c3, 1);
    EXPECT_EQ( testAgent.threshold, 1);
    EXPECT_EQ( testAgent.opinion, 0);
    EXPECT_EQ( testAgent.alpha, 0);
    EXPECT_EQ( testAgent.sigma, 0.1);
    EXPECT_EQ( testAgent.g, 0);
    EXPECT_EQ( testAgent.u, 0);
    EXPECT_EQ( testAgent.oldU, 0);
    EXPECT_EQ( testAgent.marketDepth, 0);
    EXPECT_EQ( testAgent.countNeighbours, 0);
    EXPECT_EQ( testAgent.personalInformation, 0);
    EXPECT_EQ( testAgent.neighbourhoodInformation, 0);
    EXPECT_EQ( testAgent.kr, 0);
    EXPECT_EQ( testAgent.prevExcessDemand ,  0);
    EXPECT_EQ( testAgent.marketDepth ,  0);

    delete randomGenerator;
}
