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

#ifndef PRICECALCULATOR_PRICECALCULATORGENERAL_H_
#define PRICECALCULATOR_PRICECALCULATORGENERAL_H_

#include "PriceCalculator.h"
#include "../RandomGenerator/RandomGenerator.h"
#include <string>

class PriceCalculatorGeneral: public PriceCalculator {
private:
	RandomGenerator* randomGenerator;
	std::string fFunction;
    double fConstant;
	std::string gFunction;
    double gConstant;

	//For the Calculation of ED dot
	double prevExcessDemand;

    double F();
    double G() const;
    double FConstant(double constant) const;
    double FLinear(double constant) const;
    double FHarras(double constant) const;
	double FCross();
    double FCross2() const;
    double GConstant(double constant) const;
    double GLinear(double constant) const;
	double GCross(double constant) const;

public:
	PriceCalculatorGeneral();

	virtual ~PriceCalculatorGeneral();

	PriceCalculatorGeneral(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice, ExcessDemand* newExcessDemand);
	PriceCalculatorGeneral(ExcessDemandCalculator* newExcessDemandCalculator, RandomGenerator* newRandomGenerator,
						   Price* newPrice, ExcessDemand* newExcessDemand);

	 virtual void  preStepCalculate() ;

	 virtual void  stepCalculate() ;

	 virtual void  postStepCalculate() ;

    void setFFunction(const std::string& newFFunction);
    void setFConstant(double newFConstant);
    void setGFunction(const std::string& newGFunction);
    void setGConstant(double newGConstant);

};

#endif /* PRICECALCULATOR_PRICECALCULATORGENERAL_H_ */
