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
 * @file ExcessDemandCalculatorLLS.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef EXCESSDEMANDCALCULATOR_EXCESSDEMANDCALCULATORLLS_H_
#define EXCESSDEMANDCALCULATOR_EXCESSDEMANDCALCULATORLLS_H_

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

#include <map>
#include <boost/assign.hpp>

#include "ExcessDemandCalculator.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/Dividend.h"
#include "../Agent/Agent.h"


enum class LLSEDMode
{
    original,
    substractInterest,
    relaxExp,
    substractLastED,
    substractExpectedVolume
};

const std::map<std::string, LLSEDMode> stringToLLSEDMode =
        boost::assign::map_list_of("original", LLSEDMode::original)
        ("substractinterest", LLSEDMode::substractInterest)
        ("relaxexp", LLSEDMode::relaxExp)
        ("substractlasted", LLSEDMode::substractLastED)
        ("substractexpectedvolume", LLSEDMode::substractExpectedVolume);

/** Calculates the excess demand with the formula from "How to grow a bubble: A model of myopic adapting agents" by
 * Georges LLS and Didier Sornette. Maybe Torsten can give a general name for this method.
 * Implements ExcessDemandCalculator.
 */
class ExcessDemandCalculatorLLS: public ExcessDemandCalculator {
public:
	enum class LLSEDMode
	{
		original,
		substractInterest,
		relaxExp,
		substractLastED,
		substractExpectedVolume,
		normalize
	};
	ExcessDemandCalculatorLLS(std::vector<Agent*>* newAgents, ExcessDemand* newExcessDemand,
							  Price* price, Dividend* dividend, std::string mode, double stocksPerAgent);

	virtual ~ExcessDemandCalculatorLLS();

	virtual void  preStepCalculate() ;

	virtual void  stepCalculate() ;

	virtual void  postStepCalculate() ;

	void setTotalAmountOfStock(double newTotalAmountOfStock);



	static const std::map<std::string, LLSEDMode> stringToLLSEDMode;
private:
	double tempExcessDemand; /** Temporary variable used in computation. */
    double stockPerAgent;
	double totalAmountOfStock;
	double oldPrice;
    Price* price;
    Dividend* dividend;
    LLSEDMode mode;




};

#endif /* EXCESSDEMANDCALCULATOR_EXCESSDEMANDCALCULATORLLSS_H_ */
