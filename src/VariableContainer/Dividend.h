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


//TODO: If more dividend processes are implemented, change to DividendCalculator

#ifndef VARIABLECONTAINERS_Dividend_H_
#define VARIABLECONTAINERS_Dividend_H_

#include "../RandomGenerator/RandomGenerator.h"
#include "DeltaT.h"


class Simulation;
class StockExchange;


/** Container Class for the Dividend. Can be included in every class that needs access to the current Dividend.
 */
class Dividend{
friend Simulation;
friend StockExchange;
#if BUILD_TESTS
    FRIEND_TEST(fullSimulationTest, fullSimulation_RII);
    FRIEND_TEST(fullSimulationTest, fullSimulation_EMB);
#endif
private:
	RandomGenerator* randomGenerator;
	DeltaT* deltaT;
	double dividend; /**< Holds the current Dividend */

	double z1;
	double z2;
    double expectedIncrease;

	void calculateDividend();
	void setZ(double newZ1, double newZ2);

public:
	Dividend();
	Dividend(RandomGenerator* newRandomGenerator, DeltaT* newDeltaT);
	Dividend(RandomGenerator* newRandomGenerator, DeltaT* newDeltaT,
			 double newZ1, double newZ2, double newInitialDividend);
	virtual ~Dividend();

    const double& getDividend() const;
    const double& getExpectedIncrease() const;
    const double& getZ1() const;
    const double& getZ2() const;
};

#endif /* VARIABLECONTAINERS_Dividend_H_ */
