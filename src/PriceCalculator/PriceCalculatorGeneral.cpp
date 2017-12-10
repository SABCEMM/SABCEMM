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

#include "PriceCalculatorGeneral.h"


/** Standarddestructor
 */
PriceCalculatorGeneral::~PriceCalculatorGeneral() = default;


/** Standardconstructor
 */
PriceCalculatorGeneral::PriceCalculatorGeneral():PriceCalculatorGeneral(nullptr, nullptr, nullptr, nullptr){
}


/** Constructor for the PriceCalculatorGeneral. For Documentation see parent class PriceCalculator.
 */
PriceCalculatorGeneral::PriceCalculatorGeneral(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice,
											   ExcessDemand* newExcessDemand):
		PriceCalculatorGeneral(newExcessDemandCalculator, nullptr, newPrice, newExcessDemand){
}

PriceCalculatorGeneral::PriceCalculatorGeneral(ExcessDemandCalculator* newExcessDemandCalculator,
											   RandomGenerator* newRandomGenerator,
											   Price* newPrice,
											   ExcessDemand* newExcessDemand):
                                         PriceCalculator(newExcessDemandCalculator, newPrice, newExcessDemand)
                                         , randomGenerator(newRandomGenerator)
                                         , fFunction("")
                                         , fConstant(0)
                                         , gFunction("")
                                         , gConstant(0)
{
    prevExcessDemand = 0;
}


/** Calculate the new Price
 * ExcessDemandCalculator is called first.
 */
void PriceCalculatorGeneral::stepCalculate() {

	assert(randomGenerator != nullptr);
	assert(excessDemandCalculator != nullptr);
	assert(excessDemand != nullptr);
	assert(price != nullptr);

	double eta = 0;
	double newPrice = 0;

	excessDemandCalculator->stepCalculate();
	randomGenerator->getNormalRandomDouble(0,1,&eta);

	newPrice  = price->getPrice() + deltaT->getDeltaT() * F() + sqrt(deltaT->getDeltaT()) * G() * eta;

	price->setPrice(newPrice);


}

void PriceCalculatorGeneral::preStepCalculate(){

}
void PriceCalculatorGeneral::postStepCalculate(){

}

double PriceCalculatorGeneral::F(){

	if(fFunction == string("FConstant")){
        return FConstant(fConstant);
	}
	else if(fFunction == string("FHarras")){
		return FHarras(fConstant);
	}
	else if(fFunction == string("FLinear")){
        return FLinear(fConstant);
	}
	else if(fFunction == string("FCross")){
		return FCross();
	}
    else if(fFunction == string("FCross2")){
        return FCross2();
    }
	else{
		throw("PriceCalculatorGeneral: fFunction unknown!");
	}
	return 0;
}

double PriceCalculatorGeneral::G() const{

	if(gFunction == string("GConstant")){
        return GConstant(gConstant);
	}
	else if(gFunction == string("GLinear")){
		return GLinear(gConstant);
	}
	else if(gFunction == string("GCross")){
		return GCross(gConstant);
	}
	else{
		throw("PriceCalculatorGeneral: gFunction unknown!");
	}
	return 0;
}

double PriceCalculatorGeneral::FConstant(double constant) const{

	return excessDemand->getExcessDemand() * constant / marketDepth;
}

double PriceCalculatorGeneral::FLinear(double constant) const{

	return excessDemand->getExcessDemand() * price->getPrice() * constant / marketDepth;
}

double PriceCalculatorGeneral::FHarras(double constant) const{

    //return price->getPrice() * (expm1(excessDemand->getExcessDemand() / marketDepth));
	// Die Konstante ist ein Pseudopreisschritt
    // Im ODE Kontext:
    return price->getPrice() * excessDemand->getExcessDemand() * constant / marketDepth;
}

double PriceCalculatorGeneral::FCross(){

    // TODO: Ist deltaT hier richtig berechnet? Oder doch nur als excessDemand->getExcessDemand() - prevExcessDemand??
    double dED_dt = (excessDemand->getExcessDemand() - prevExcessDemand) / deltaT->getDeltaT();
    prevExcessDemand = excessDemand->getExcessDemand();

    return marketDepth * dED_dt * price->getPrice();
}

double PriceCalculatorGeneral::FCross2() const{

    // TODO: Quasi wie FLinear nur mit mal marketDepth anstatt durch marketDepth

    return excessDemand->getExcessDemand() * price->getPrice() * marketDepth;
}

double PriceCalculatorGeneral::GConstant(double constant) const{

	 return constant;
}

double PriceCalculatorGeneral::GLinear(double constant) const{

	 return price->getPrice() * constant;
}

double PriceCalculatorGeneral::GCross(double constant) const{

	return (1+ constant * fabs(excessDemand->getExcessDemand())) * price->getPrice();
}

void PriceCalculatorGeneral::setFFunction(const string &newFFunction){

	fFunction = newFFunction;
}

void PriceCalculatorGeneral::setFConstant(double newFConstant){

    fConstant = newFConstant;
}

void PriceCalculatorGeneral::setGFunction(const string &newGFunction){

	gFunction = newGFunction;
}

void PriceCalculatorGeneral::setGConstant(double newGConstant){

    gConstant = newGConstant;
}

