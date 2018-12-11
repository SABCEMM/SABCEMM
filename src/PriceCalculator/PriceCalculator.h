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

#ifndef _PriceCalculator_hpp_
#define _PriceCalculator_hpp_

#include "../Input/Input.h"
#include "../ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/DeltaT.h"
#include "../RandomGenerator/RandomGenerator.h"

// forward declaration
class StockExchange;



/** Virtual parent class for all PriceCalculator.
 * This class defines the public interface for all PriceCalculators. It relies on the ExcessDemandCalculator
 * to calculate a new price.
 * Can only be constructed if an ExcessDemandCalculator is provided.
 */
class PriceCalculator {
    friend StockExchange;

protected:
	ExcessDemandCalculator* excessDemandCalculator;	/**< Pointer to an ExcessDemandCalculator object. */
	Price* price;	/**< Holds the last calculated price for later reference. */
	ExcessDemand* excessDemand; /**< Pointer to the ExcessDemand container */
    DeltaT* deltaT;
	double marketDepth; /**< Lambda from the Harras model and Kappa in the Cross model */


public:
	PriceCalculator();
	PriceCalculator(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice, ExcessDemand* newExcessDemand);
    static PriceCalculator* factory(Input& input, ExcessDemandCalculator* excessDemandCalculator,
                                    Price* price, ExcessDemand* excessDemand, DeltaT* deltaT,
                                    RandomGenerator* randomNumberPool, std::vector<Agent*>* agents);
	virtual ~PriceCalculator();

	void setDeltaT(DeltaT* newDeltaT);
	void setMarketDepth(double newMarketDepth);
	void setExcessDemandCalculator(ExcessDemandCalculator* newExcessDemandCalculator);

	virtual void preStepCalculate() = 0;
	virtual void stepCalculate() = 0;
	virtual void postStepCalculate() = 0;
};



#endif
