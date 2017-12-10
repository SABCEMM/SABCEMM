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

#include <cmath>

#include "PriceCalculatorHarras.h"


/** Standarddestructor
 */
PriceCalculatorHarras::~PriceCalculatorHarras() = default;


/** Calculate the new Price according to the paper "How to grow a bubble: A model of myopic adapting agents" by
 * Georges Harras and Didier Sornette. ExcessDemandCalculator is called first.
 */
void PriceCalculatorHarras::stepCalculate() {
	assert(excessDemandCalculator != nullptr);
	assert(excessDemand != nullptr);
	assert(price != nullptr);

	excessDemandCalculator->stepCalculate();
	double oldPrice = price->getPrice();
	price->setPrice( exp(log(oldPrice) + deltaT->getDeltaT() * excessDemand->getExcessDemand() / marketDepth) );


}

void PriceCalculatorHarras::preStepCalculate(){

}
void PriceCalculatorHarras::postStepCalculate(){

}


/** Standardconstructor
 */
PriceCalculatorHarras::PriceCalculatorHarras():PriceCalculatorHarras(nullptr, nullptr, nullptr){
}

/** Constructor for the PriceCalculatorHarras. For Documentation see parent class PriceCalculator.
 */
PriceCalculatorHarras::PriceCalculatorHarras(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice,
											 ExcessDemand* newExcessDemand):
		PriceCalculator(newExcessDemandCalculator, newPrice, newExcessDemand){
}
