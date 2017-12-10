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
#include "Dividend.h"
#include <cassert>


/** Constructor for the Dividend container that is initialized to 1.
 */
Dividend::Dividend():Dividend(nullptr,nullptr){

}

Dividend::Dividend(RandomGenerator* newRandomGenerator, DeltaT* newDeltaT):
		Dividend(newRandomGenerator, newDeltaT,
				 0, 1, 0)
{

}

Dividend::Dividend(RandomGenerator* newRandomGenerator, DeltaT* newDeltaT,
				   double newZ1, double newZ2, double newInitialDividend):
		randomGenerator(newRandomGenerator),
        deltaT(newDeltaT),
		dividend(newInitialDividend)
{
	setZ(newZ1, newZ2);
}


/** Destructor
 */
Dividend::~Dividend() {
	// TODO Auto-generated destructor stub
}


/// Setter method for the Dividend. Is private and only callable by befriended implementations
/// of the DividendCalculator.
void Dividend::calculateDividend(){
	assert(randomGenerator != nullptr);
	assert(deltaT != nullptr);

	double eta = 0;
	randomGenerator->getUniformRandomDouble(z1,z2,&eta); /// @todo According to Torsten:
/// Truncated random variable? Which distribution?

	dividend = (1 + deltaT->getDeltaT() * eta) * dividend;
}


/** Getter method for the current Dividend.
 * \return const reference to the current Dividend
 */
const double& Dividend::getDividend() const{
	return dividend;
}

/** Sets the bounds of the uniform distribution and calculates the expected value.
 *
 */
void Dividend::setZ(double newZ1, double newZ2){
	assert(newZ2>=newZ1);
	z1 = newZ1;
	z2 = newZ2;
	expectedValue = (z1+z2)/2;
}

const double& Dividend::getExpectedValue() const{
	return expectedValue;
}

const double& Dividend::getZ1() const{
	return z1;
}
const double& Dividend::getZ2() const{
	return z2;
}
