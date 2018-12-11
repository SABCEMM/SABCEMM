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
#include <cassert>
#include "PriceCalculatorHarrasNoise.h"


/** Standarddestructor
 */
PriceCalculatorHarrasNoise::~PriceCalculatorHarrasNoise() = default;


/** Calculate the new Price according to the paper "How to grow a bubble: A model of myopic adapting agents" by
 * Georges Harras and Didier Sornette. ExcessDemandCalculator is called first.
 */
void PriceCalculatorHarrasNoise::stepCalculate() {
	assert(excessDemandCalculator != nullptr);
	assert(excessDemand != nullptr);
	assert(price != nullptr);

    //TODO: Mit PriceCalculatorHarras zusammenfassen. Als Noise Mode einfach None oder Original einführen.

	excessDemandCalculator->stepCalculate();
	double oldPrice = price->getPrice();

    double eta = 0;
    randomGenerator->getNormalRandomDouble(0,1,&eta);

    switch(mode)
    {
        case ADD:
            price->setPrice( exp(log(oldPrice) + deltaT->getDeltaT() * excessDemand->getExcessDemand()
                                                 / marketDepth + constant * eta) );
            break;
        case MULT: //Angelehnt an Cross
            price->setPrice( exp(log(oldPrice) + deltaT->getDeltaT() * excessDemand->getExcessDemand() / marketDepth +
                                         constant * (1+fabs(excessDemand->getExcessDemand())*theta) * eta) );
    }

}

void PriceCalculatorHarrasNoise::preStepCalculate(){

}
void PriceCalculatorHarrasNoise::postStepCalculate(){

}


/** Standardconstructor
 */
PriceCalculatorHarrasNoise::PriceCalculatorHarrasNoise():PriceCalculatorHarrasNoise(nullptr, nullptr, nullptr, ADD,
                                                                                    0, 0, nullptr){
}

/** Constructor for the PriceCalculatorHarras. For Documentation see parent class PriceCalculator.
 */
PriceCalculatorHarrasNoise::PriceCalculatorHarrasNoise(ExcessDemandCalculator* newExcessDemandCalculator,
                                                       Price* newPrice, ExcessDemand* newExcessDemand,
                                                       NoiseMode mode, double constant, double theta,
                                                       RandomGenerator* randomGenerator):
		PriceCalculator(newExcessDemandCalculator, newPrice, newExcessDemand){
    this->mode = mode;
    this->constant = constant;
    this->randomGenerator = randomGenerator;
    this->theta = theta;
}
