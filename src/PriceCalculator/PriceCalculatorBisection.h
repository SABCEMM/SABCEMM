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

#ifndef PRICECALCULATOR_BISECTION_H_
#define PRICECALCULATOR_BISECTION_H_

#include "PriceCalculator.h"
#include "../Agent/Agent.h"
#include "../Exceptions/NoConvergenceException.h"
#include "../Exceptions/BadDomainError.h"

#include <vector>
#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * Using a bisection, the price is chosen such that the excess demand at that price approaches zero.
 */
class PriceCalculatorBisection: public PriceCalculator {
#if BUILD_TESTS
    FRIEND_TEST(PriceCalculatorBisection, getExcessDemandAtPrice);
    FRIEND_TEST(PriceCalculatorBisection, stepCalculate);
#endif
private:
    /// Defines a tolerance. Prices, such that abs(excessDemand) < epsilon, are considered valid.
	double epsilon;
    /// Bisection will fail if solution is solution is after this amount of iterations.
    /// A failing bisection will terminate the program.
    std::size_t maxIterations;

    /// Controls whether adaptive bounds are active.
    /// With adaptive bounds, the price is sought within the interval [low*oldPrice, high*oldPrice].
    /// Without adaptive bounds, the price is sought within the interval [low, high].
    bool adaptive;
    /// @see adaptive
    double low;
    /// @see adaptive
    double high;

    /// agents of the current simulation
	std::vector<Agent*>* agents;

    /// Returns the excess demand at the specified price.
	double getExcessDemandAtPrice(double iterPrice);

public:
    PriceCalculatorBisection() = default;
    PriceCalculatorBisection(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice, ExcessDemand* newExcessDemand, bool adaptive, double low, double high, double epsilon, std::size_t maxIterations);

	virtual ~PriceCalculatorBisection();

	virtual void preStepCalculate();

	virtual void stepCalculate();

	virtual void postStepCalculate();

	void setAgents(std::vector<Agent*>* newAgents);


};

#endif /* PRICECALCULATOR_BISECTION_H_ */
