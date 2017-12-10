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

#ifndef VARIABLECONTAINERS_PRICE_H_
#define VARIABLECONTAINERS_PRICE_H_

#if BUILD_TESTS
#include "gtest/gtest_prod.h"

#endif




class PriceCalculatorBisection;
class PriceCalculatorCross;
class PriceCalculatorFW;
class PriceCalculatorGeneral;
class PriceCalculatorHarras;
class PriceCalculatorHarrasNoise;
class PriceCalculatorLLS;
class Simulation;
class VariableContainer;

/** Container Class for the Price. Can be included in every class that needs access to the current price.
 * The excess demand can only be set by befriended implementations of the PriceCalculator.
 */

class Price{
    /// @todo friend entfernen, wenn einfach möglich.
	friend PriceCalculatorBisection;
    friend PriceCalculatorCross;
    friend PriceCalculatorFW;
    friend PriceCalculatorGeneral;
    friend PriceCalculatorHarras;
    friend PriceCalculatorHarrasNoise;
	friend Simulation;
	friend PriceCalculatorLLS;
	friend VariableContainer;
#if BUILD_TESTS
	FRIEND_TEST(DataItemCollectorWealth, collectData);
	FRIEND_TEST(DataItemCollectorWealth, collectDataPerAgent);
	FRIEND_TEST(DataItemCollectorWealth, collectData_with1Group);
	FRIEND_TEST(DataItemCollectorWealth, collectData_with2Group);
	FRIEND_TEST(DataItemCollectorWealth, collectData_with2GroupPerAgent);
	FRIEND_TEST(DataItemCollectorPrice, collectData);
	FRIEND_TEST(AgentCrossTest, constructor);
	FRIEND_TEST(AgentCrossTest, stepUpdate);
	FRIEND_TEST(AgentCrossTest, updateBisection);
#endif

private:
	double price; /**< Holds the current price */
	void setPrice(double price);

public:
	Price();
	virtual ~Price();

    const double& getPrice() const;
};

#endif /* VARIABLECONTAINERS_PRICE_H_ */
