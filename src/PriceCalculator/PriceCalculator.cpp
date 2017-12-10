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

#include "PriceCalculator.h"
#include "PriceCalculatorBisection.h"
#include "PriceCalculatorCross.h"
#include "PriceCalculatorGeneral.h"
#include "PriceCalculatorHarras.h"
#include "PriceCalculatorLLS.h"
#include "PriceCalculatorHarrasNoise.h"
#include "PriceCalculatorFW.h"
#include "../Parameter/Parameter.h"

#include "../VariableContainer/Price.h"
#include "../VariableContainer/DeltaT.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../RandomGenerator/RandomGenerator.h"
#include "../Parameter/ParameterSetPriceCalculator.h"



/** Standardconstructor for the PriceCalculator.
 */
PriceCalculator::PriceCalculator(): PriceCalculator(nullptr, nullptr, nullptr){
}


/** Constructor for the pure-virtual PriceCalculator.
 * Requires an ExcessDemandCalculator for the object to work properly.
 * \param newExcessDemandCalculator Pointer to an ExcessDemandCalculator object
 * \param newPrice Pointer to the Price container
 * \param newExcessDemand Pointer to the ExcessDemand container
 */
PriceCalculator::PriceCalculator(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice,
								 ExcessDemand* newExcessDemand) {
	excessDemandCalculator = newExcessDemandCalculator;
	price = newPrice;
	excessDemand = newExcessDemand;
	deltaT = nullptr;
	marketDepth = 0.25;
}

PriceCalculator* PriceCalculator::factory(Parameter* parameter, ExcessDemandCalculator* excessDemandCalculator,
										  Price* price, ExcessDemand* excessDemand, DeltaT* deltaT,
                                          RandomGenerator* randomNumberPool, std::vector<Agent*>* agents){
	//priceCalculator
	ParameterSetPriceCalculator parameterSetPriceCalculator = parameter->parameterSetPriceCalculator;
	string type = parameterSetPriceCalculator.type.get();

	PriceCalculator* priceCalculator;

	if (type == "PriceCalculatorHarras") {
		priceCalculator = new PriceCalculatorHarras(excessDemandCalculator,price, excessDemand);
		priceCalculator->setMarketDepth(*(parameterSetPriceCalculator.marketDepth));
	}
	else if (type == "PriceCalculatorHarrasAdd") {
		priceCalculator = new PriceCalculatorHarrasNoise(excessDemandCalculator,price, excessDemand,
														 PriceCalculatorHarrasNoise::ADD,
														 *(parameterSetPriceCalculator.noiseFactor),
														 0, randomNumberPool);
		priceCalculator->setMarketDepth(*(parameterSetPriceCalculator.marketDepth));
	}
	else if (type == "PriceCalculatorHarrasMult") { //TODO: 0 ist theta!!
		priceCalculator = new PriceCalculatorHarrasNoise(excessDemandCalculator,price, excessDemand,
														 PriceCalculatorHarrasNoise::MULT,
														 *(parameterSetPriceCalculator.noiseFactor),
														 *(parameterSetPriceCalculator.theta), randomNumberPool);
		priceCalculator->setMarketDepth(*(parameterSetPriceCalculator.marketDepth));
	}
		//priceCalculator General
	else if (type == "PriceCalculatorGeneral") {
		auto * calculator = new PriceCalculatorGeneral(
				excessDemandCalculator, randomNumberPool, price, excessDemand);
		calculator->setFFunction(*parameterSetPriceCalculator.fFunction);
		/// @todo calculator should do this check
		if (parameterSetPriceCalculator.fConstant){
			calculator->setFConstant(*parameterSetPriceCalculator.fConstant);
		}

		calculator->setGFunction(*parameterSetPriceCalculator.gFunction);
		if (parameterSetPriceCalculator.gConstant){
			calculator->setGConstant(*parameterSetPriceCalculator.gConstant);
		}

		priceCalculator = calculator;
		priceCalculator->setMarketDepth(*(parameterSetPriceCalculator.marketDepth));

	}
		//priceCalculator Bisection
	else if (type== "PriceCalculatorBisection") {
		priceCalculator = new PriceCalculatorBisection(excessDemandCalculator,price, excessDemand);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setEpsilon(*parameterSetPriceCalculator.epsilon);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setMaxIterations(
				*parameterSetPriceCalculator.maxIterations);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setBounds(*parameterSetPriceCalculator.lowerBound,
																			  *parameterSetPriceCalculator.upperBound);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setAgents(agents);
	}
		//priceCalculator LLS
	else if (type== "PriceCalculatorLLS") {
		priceCalculator = new PriceCalculatorLLS(excessDemandCalculator,price, excessDemand);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setEpsilon(*parameterSetPriceCalculator.epsilon);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setMaxIterations(
				*parameterSetPriceCalculator.maxIterations);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setBounds(*parameterSetPriceCalculator.lowerBound,
																			  *parameterSetPriceCalculator.upperBound);
		(dynamic_cast<PriceCalculatorBisection*>(priceCalculator))->setAgents(agents);
	}
		//priceCalculator Cross
	else if (type == "PriceCalculatorCross") {
		priceCalculator = new PriceCalculatorCross(excessDemandCalculator,price, excessDemand, randomNumberPool,
												   PriceCalculatorCross::ORIGINAL);
		(dynamic_cast<PriceCalculatorCross*>(priceCalculator))->setTheta(*(parameterSetPriceCalculator.theta));
		priceCalculator->setMarketDepth(*(parameterSetPriceCalculator.marketDepth));
	}
	else if (type == "PriceCalculatorCrossMartingale" ){
		priceCalculator = new PriceCalculatorCross(excessDemandCalculator,price, excessDemand, randomNumberPool,
												   PriceCalculatorCross::MARTINGALE);
		(dynamic_cast<PriceCalculatorCross*>(priceCalculator))->setTheta(*(parameterSetPriceCalculator.theta));
		priceCalculator->setMarketDepth(*(parameterSetPriceCalculator.marketDepth));
	}
	else if (type == "PriceCalculatorFW"){
		priceCalculator = new PriceCalculatorFW(excessDemandCalculator, price,
                                                excessDemand, *(parameterSetPriceCalculator.mu));
	}

	else {
		throw("priceCalculatorClass unknown!");
	}

	priceCalculator->setDeltaT(deltaT);
	return priceCalculator;
}

/** Virtual destructor of the PriceCalculator.
 */
PriceCalculator::~PriceCalculator() = default;


/** Setter method for private excessDemandCalculator
 * \param newExcessDemandCalculator Pointer to an ExcessDemandCalculator object
 */
void PriceCalculator::setExcessDemandCalculator(ExcessDemandCalculator* newExcessDemandCalculator) {
	excessDemandCalculator = newExcessDemandCalculator;
}

void PriceCalculator::setDeltaT(DeltaT* newDeltaT){
	deltaT = newDeltaT;
}

void PriceCalculator::setMarketDepth(double newMarketDepth){
	marketDepth = newMarketDepth;
}
