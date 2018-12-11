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

#ifndef _PriceCalculatorCross_hpp_
#define _PriceCalculatorCross_hpp_

#include "../ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../RandomGenerator/RandomGenerator.h"
#include "../PriceCalculator/PriceCalculator.h"

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

/** Calculates the price as presented in the Cross model. Refer to CalculationMethod for further information.
 */
class PriceCalculatorCross : public PriceCalculator{
#if BUILD_TESTS
    FRIEND_TEST(fullSimulationTest, fullSimulation_Cross);
#endif



public:
    /// Differentiates between two variantes of Cross price calculation.
    enum CalculationMethod
    {
        /// Calculate the price as in Cross et al.: "A Threshold Model of Investor Psychology", eq.3.2
        ORIGINAL,
        /**
         * Calculate the price as in Cross et al.: "Stylized facts from a threshold-based heterogeneous agent model", eq.2
         *
         * Additionally, heteroskedacity is taken into account. To eliminate heteroskedacity (which then really gives
         * the equation as in the paper), choose theta = 0.
         */
        MARTINGALE
    };

    explicit PriceCalculatorCross(CalculationMethod method);
    PriceCalculatorCross(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice, ExcessDemand* newExcessDemand, RandomGenerator* newRandomGenerator, CalculationMethod method);

    virtual ~PriceCalculatorCross();

     virtual void  preStepCalculate() ;

     virtual void  stepCalculate() ;

     virtual void  postStepCalculate() ;

    void setTheta(double theta);

private:
    /// a parameter proportional to the influence of the excess demand on the price.
    /// Corresponds to the parameter alpha in eq. 3.2.
    double theta;
    /// the latest (current) excess demand.
    double prevExcessDemand;

    RandomGenerator* randomGenerator;

    CalculationMethod method;
};


#endif
