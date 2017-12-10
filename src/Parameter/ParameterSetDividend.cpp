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

#include "ParameterSetDividend.h"

using namespace std;
using namespace tinyxml2;

/** Performs a consistency check on the ParameterSetDividend object.
 * It only checks if all necessary parameters are set, it is NOT checked weather the parameters make sens.
 * This is done at object creation/initialisation mostly in the Simulation.cpp.
 * \return True if all necessary parameters are set, False if some parameters are missing.
 */
bool ParameterSetDividend::validate(){
    //
#define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}

    // Test General Arguments
    REQUIRED(interestRate);
    if(Z1 || Z2 || initialDividend){
        REQUIRED(Z1);
        REQUIRED(Z2);
        REQUIRED(initialDividend);
    }

    return true;
}

/** Read the content of the XML Block and generate valid ParameterSetDividend objetcs.
 * ParameterSet objects are not checked for consistency. This can be done via validate().
 * \note All known parameters are read from the file. Additional/Unnecessary parameters should be ignored/go unused
 * in the simulation.
 * \param dividend XMLElement as parsed in XML from file
 * \return ParameterSetDividend object
 */
ParameterSetDividend ParameterSetDividend::readFromXML(XMLElement* dividend){
    ParameterSetDividend parameterSetDividend;

    if(dividend){
        if(XMLElement* Z1Element =  dividend->FirstChildElement("Z1"))
        {
            double Z1 = 0;
            Z1Element->QueryDoubleText(&Z1);
            parameterSetDividend.Z1 = Z1;
        }
        if(XMLElement* Z2Element =  dividend->FirstChildElement("Z2"))
        {
            double Z2 = 0;
            Z2Element->QueryDoubleText(&Z2);
            parameterSetDividend.Z2 = Z2;
        }
        if(XMLElement* interestRateElement =  dividend->FirstChildElement("interestRate"))
        {
            double interestRate = 0;
            interestRateElement->QueryDoubleText(&interestRate);
            parameterSetDividend.interestRate = interestRate;
        }
        if(XMLElement* initialDividendElement =  dividend->FirstChildElement("initialDividend"))
        {
            double initialDividend = 0;
            initialDividendElement->QueryDoubleText(&initialDividend);
            parameterSetDividend.initialDividend = initialDividend;
        }
    }

    return parameterSetDividend;
}