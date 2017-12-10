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
 * @file AgentEMB.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef _AgentEMB_hpp_
#define _AgentEMB_hpp_

#include <vector>
#include <cstddef> //for std::size_t
#include "Agent.h"
#include "../RandomGenerator/RandomGenerator.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/GlobalNews.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/Dividend.h"

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

/** Agent from the paper CrossAgents
 */
class AgentEMB : public Agent {
protected:
	double riskTolerance; //Alpha in the Utility Function
    std::size_t memorySpan; //How far does the agent look back
	double stdNoiseSigma; //Standard deviation for the noise on sigma
	double interestRate;

    double oldGamma;
    double gamma;
	double iterGamma;
	double hypoCash;

        /// @todo replace with a deque for pop_front (faster!), that requires making
        ///       RandomGenerator::getUniformRandomDouble(...) a template.
	vector<double> relPriceHistory; //relative stock price change time series
	double oldPrice;

    Dividend* dividend; /**< Pointer to the dividend container */

	void updateCash();
	void updateTradingVolume(const double& newGamma, const double& newPrice, const double& newHypoCash);
	double utility(double newCash);
    double expectedUtility(double newGamma);
    /// first derivative of EU.
    double dExpectedUtility(double gamma);
public:
	AgentEMB();
	AgentEMB(RandomGenerator* newRandomGenerator, Price* newPrice);
	AgentEMB(RandomGenerator* newRandomGenerator, Price* newPrice,
			double newCash, double newStock);

	AgentEMB(RandomGenerator* newRandomGenerator, Price* newPrice,
			 double newCash, double newStock,
			 Dividend* newDividend, double newRiskTolerance,
			 std::size_t newMemorySpan, double newStdNoiseSigma,
			 double newInterestRate, double historyMean,
			 double historySigma, double initialGamma);

	 virtual void  preStepUpdate() ;

	 virtual void  stepUpdate() ;

	 virtual void  postStepUpdate() ;

	 virtual void  updateBisection(const double& newIterPrice) ;

    double getOldGamma(){return oldGamma;}
    double getGamma(){return gamma;}
    double getIterGamma(){return iterGamma;}
    double getInterestRate(){return interestRate;}
    double getStock(){return stock;}

};

#endif
