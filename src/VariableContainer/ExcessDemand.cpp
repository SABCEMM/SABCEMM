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

#include "ExcessDemand.h"






/** Constructor for the ExcessDemand. Default: Initiliaze excess demand to 0.
 */
ExcessDemand::ExcessDemand(double newExcessDemand) {
	excessDemand = newExcessDemand;
	term1LLS = 0;
	term2LLS = 0;
}


/** Destructor
 */
ExcessDemand::~ExcessDemand() {
	// TODO Auto-generated destructor stub
}


/** Setter method for the excessDemand. Is private and only callable by befriended implementations
 * of the ExcessDemandCalculator.
 * \param newExcessDemand Reference to the new excess demand
 */
void ExcessDemand::setExcessDemand(double newExcessDemand){
	excessDemand = newExcessDemand;
}


/** Getter method for the excessDemand.
 * \return Returns the excessDemand.
 */
const double& ExcessDemand::getExcessDemand() const{
	return excessDemand;
}

void ExcessDemand::setLLSinfo(double term1, double term2){
	term1LLS = term1;
	term2LLS = term2;
}

void ExcessDemand::getLLSinfo(double& term1, double& term2){
	term1 = term1LLS;
	term2 = term2LLS;
}