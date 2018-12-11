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
#include "PriceCalculatorCross.h"


/** Standardconstructor for the PriceCalculatorCross.
 */
PriceCalculatorCross::PriceCalculatorCross(CalculationMethod method): PriceCalculatorCross(nullptr, nullptr, nullptr, nullptr, method){
}


/** Constructor for the pure-virtual PriceCalculatorCross.
 * Requires an ExcessDemandCalculator for the object to work properly.
 * \param newExcessDemandCalculator Pointer to an ExcessDemandCalculator object
 * \param newPrice Pointer to the Price container
 * \param newExcessDemand Pointer to the ExcessDemand container
 */
PriceCalculatorCross::PriceCalculatorCross(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice, ExcessDemand* newExcessDemand, RandomGenerator *newRandomGenerator, CalculationMethod method):
    PriceCalculator(newExcessDemandCalculator, newPrice, newExcessDemand), randomGenerator(newRandomGenerator), method(method){
    /// @todo Setter für theta eliminieren und stattdessen im Konstruktor setzen.
	theta = 2;
	prevExcessDemand = 0;
    /// @todo ACHTUNG: Wie muss der prevExcessDemand gesetzt werden? RandomInit zwischen [-1,1] ? TODO
    // randomGenerator->getUniformRandomDouble(-1,1,&prevExcessDemand);
}


/** Virtual destructor of the PriceCalculatorCross.
 */
PriceCalculatorCross::~PriceCalculatorCross() = default;

void PriceCalculatorCross::stepCalculate(){
	assert(excessDemandCalculator != nullptr);
	assert(excessDemand != nullptr);
	assert(price != nullptr);
	assert(randomGenerator != nullptr);

	double tempPrice = 0;
	double eta = 0;
	double deltaED = 0;

	excessDemandCalculator->stepCalculate();

	randomGenerator->getNormalRandomDouble(0, sqrt(deltaT->getDeltaT()), &eta); // sqrt(deltaT) * eta aus N(0,1)

	deltaED = excessDemand->getExcessDemand() - prevExcessDemand;
	prevExcessDemand = excessDemand->getExcessDemand();

    switch(method)
    {
        case ORIGINAL:
            tempPrice = price->getPrice() * exp( (1+theta*fabs(excessDemand->getExcessDemand()) ) * ( eta ) + marketDepth * deltaED);
            break;
        case MARTINGALE:
            tempPrice = price->getPrice() * exp( (1+theta*fabs(excessDemand->getExcessDemand()) ) * ( eta ) - deltaT->getDeltaT()/2 + marketDepth * deltaED);
    }

	price->setPrice(tempPrice);
}

void PriceCalculatorCross::preStepCalculate(){

}
void PriceCalculatorCross::postStepCalculate(){

}

void PriceCalculatorCross::setTheta(double theta)
{
    this->theta = theta;
}
