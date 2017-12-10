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

#include <iostream>

#include "tinyxml2/tinyxml2.h"

#include "ParameterSetPriceCalculator.h"

using namespace tinyxml2;
using namespace std;

/** Performs a consistency check on the ParameterSetPriceCalculator object.
 * It only checks if all necessary parameters are set, it is NOT checked weather the parameters make sens.
 * This is done at object creation/initialisation mostly in the Simulation.cpp.
 * \return True if all necessary parameters are set, False if some parameters are missing.
 */
bool ParameterSetPriceCalculator::validate() {
#define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}

    REQUIRED(type);
// PriceCalculator Bisection
    if( *type == "PriceCalculatorBisection") {
        REQUIRED(epsilon);
        REQUIRED(maxIterations);
        REQUIRED(lowerBound);
        REQUIRED(upperBound);
    }

// PriceCalculator Cross
    if( *type == "PriceCalculatorCross"|| *type == "PriceCalculatorHarrasMult" ) {
        REQUIRED(theta);
    }

// PriceCalculator General
    if( *type == "PriceCalculatorGeneral") {
        REQUIRED(fFunction);
        REQUIRED(fConstant);
        REQUIRED(gFunction);
        REQUIRED(gConstant);
    }

// PriceCalculator Harras+General
    if( *type == "PriceCalculatorHarras" || *type == "PriceCalculatorGeneral" || *type == "PriceCalculatorCross"|| *type == "PriceCalculatorHarrasAdd" || *type == "PriceCalculatorHarrasMult" ) {
        REQUIRED(marketDepth);
    }

    if(*type == "PriceCalculatorHarrasAdd" || *type == "PriceCalculatorHarrasMult"){
        REQUIRED(noiseFactor);
    }

    if(*type == "PriceCalculatorFW")
        REQUIRED(mu);
    

// PriceCalculator LLS
//REQUIRED(totalAmountOfStock);
    return true;
}

/** Read the content of the XML Block and generate valid ParameterSetPriceCalculator objetcs.
 * ParameterSet objects are not checked for consistency. This can be done via validate().
 * \note All known parameters are read from the file. Additional/Unnecessary parameters should be ignored/go unused in the simulation.
 * \param priceCalculatorSection XMLElement as parsed in XML from file
 * \return ParameterSetPriceCalculator object
 */
ParameterSetPriceCalculator ParameterSetPriceCalculator::readFromXML(XMLElement* priceCalculatorSection){
    ParameterSetPriceCalculator parameterSetPriceCalculator;

    // Always read all possible parameters, not regarding weather required by class. additional parameters might be necessary for agent or other class...

    if(priceCalculatorSection)
    {
        if(XMLElement* priceCalculatorClassElement = priceCalculatorSection->FirstChildElement("priceCalculatorClass")) {
            parameterSetPriceCalculator.type = string(priceCalculatorClassElement->GetText());
        }


        if(XMLElement* thetaElement = priceCalculatorSection->FirstChildElement("theta"))
        {
            double theta = 0;
            thetaElement->QueryDoubleText(&theta);
            parameterSetPriceCalculator.theta = theta;
        }


        if(XMLElement* fFunctionSection = priceCalculatorSection->FirstChildElement("fFunction"))
        {
            if(XMLElement* kindElement = fFunctionSection->FirstChildElement("kind"))
            {
                parameterSetPriceCalculator.fFunction = string(kindElement->GetText());
            }
            if(XMLElement* constantElement = fFunctionSection->FirstChildElement("constant"))
            {
                double fConstant = 0;
                constantElement->QueryDoubleText(&fConstant);
                parameterSetPriceCalculator.fConstant = fConstant;
            }
        }
        if(XMLElement* gFunctionSection = priceCalculatorSection->FirstChildElement("gFunction"))
        {
            if(XMLElement* kindElement = gFunctionSection->FirstChildElement("kind"))
            {
                parameterSetPriceCalculator.gFunction = string(kindElement->GetText());
            }
            if(XMLElement* constantElement = gFunctionSection->FirstChildElement("constant"))
            {
                double gConstant = 0;
                constantElement->QueryDoubleText(&gConstant);
                parameterSetPriceCalculator.gConstant = gConstant;
            }
        }

        if(XMLElement* epsilonElement = priceCalculatorSection->FirstChildElement("epsilon"))
        {
            double epsilon = 0;
            epsilonElement->QueryDoubleText(&epsilon);
            parameterSetPriceCalculator.epsilon = epsilon;
        }
        if(XMLElement* maxIterationsElement =  priceCalculatorSection->FirstChildElement("maxIterations"))
        {
            int maxIterations = 0;
            maxIterationsElement->QueryIntText(&maxIterations);
            parameterSetPriceCalculator.maxIterations = maxIterations;
        }
        if(XMLElement* lowerBoundElement = priceCalculatorSection->FirstChildElement("lowerBound"))
        {
            double lowerBound = 0;
            lowerBoundElement->QueryDoubleText(&lowerBound);
            parameterSetPriceCalculator.lowerBound = lowerBound;
        }
        if(XMLElement* upperBoundElement = priceCalculatorSection->FirstChildElement("upperBound"))
        {
            double upperBound = 0;
            upperBoundElement->QueryDoubleText(&upperBound);
            parameterSetPriceCalculator.upperBound = upperBound;
        }

        if(XMLElement* marketDepthElement = priceCalculatorSection->FirstChildElement("marketDepth"))
        {
            double marketDepth = 0;
            marketDepthElement->QueryDoubleText(&marketDepth);
            parameterSetPriceCalculator.marketDepth = marketDepth;
        }

        if(XMLElement* noiseFactorElement = priceCalculatorSection->FirstChildElement("noiseFactor"))
        {
            double noiseFactor = 0;
            noiseFactorElement->QueryDoubleText(&noiseFactor);
            parameterSetPriceCalculator.noiseFactor = noiseFactor;
        }

        if(XMLElement* muElement = priceCalculatorSection->FirstChildElement("mu"))
        {
            double mu = 0;
            muElement->QueryDoubleText(&mu);
            parameterSetPriceCalculator.mu = mu;
        }
    }


    return parameterSetPriceCalculator;
}
