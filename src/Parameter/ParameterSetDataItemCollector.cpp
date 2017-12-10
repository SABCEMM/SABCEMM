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

#include "ParameterSetDataItemCollector.h"

#include <iostream>


using namespace std;
using namespace tinyxml2;

/** Performs a consistency check on the ParameterSetDataItemCollector object.
 * It only checks if all necessary parameters are set, it is NOT checked weather the parameters make sens.
 * This is done at object creation/initialisation mostly in the Simulation.cpp.
 * \return True if all necessary parameters are set, False if some parameters are missing.
 */
bool ParameterSetDataItemCollector::validate(){
#define REQUIRED(variable) if(!(variable)){cerr << "%%% " << #variable << " not specified. %%%" << endl; return false;}
    REQUIRED(type);

    return true;
}

/** Read the content of the XML Block and generate valid ParameterSetDataItemCollector objetcs.
 * ParameterSet objects are not checked for consistency. This can be done via validate().
 * \note All known parameters are read from the file. Additional/Unnecessary parameters should be ignored/go unused in
 * the simulation.
 * \param dataItemCollectorClassesElementXMLElement as parsed in XML from file
 * \return Vector of ParameterSetDataItemCollector objects. One for each defined DataItemCollector.
 */
vector<ParameterSetDataItemCollector> ParameterSetDataItemCollector::readFromXML
        (XMLElement* dataItemCollectorClassesElement){
    vector<ParameterSetDataItemCollector> allParameterSetDataItemCollector;

    if(dataItemCollectorClassesElement)
    {
        for(XMLElement* dataItemCollectorElement = dataItemCollectorClassesElement->FirstChildElement();
            dataItemCollectorElement != nullptr; dataItemCollectorElement=dataItemCollectorElement->NextSiblingElement())
        {
            auto * parameterSetDataItemCollector = new ParameterSetDataItemCollector();

            parameterSetDataItemCollector->type = dataItemCollectorElement->Name();

            if(XMLElement* writeIntervalClass = dataItemCollectorElement->FirstChildElement("writeInterval")) {
                int writeInterval = 0;
                writeIntervalClass->QueryIntText(&writeInterval);
                parameterSetDataItemCollector->writeInterval = writeInterval;
            }

            if(XMLElement* collectIntervalClass = dataItemCollectorElement->FirstChildElement("collectInterval")){
                int collectInterval = 0;
                collectIntervalClass->QueryIntText(&collectInterval);
                parameterSetDataItemCollector->collectInterval = collectInterval;
            }

            if(XMLElement* groupToTrackElement = dataItemCollectorElement->FirstChildElement("groupToTrack")){
                int groupToTrack = 0;
                groupToTrackElement->QueryIntText(&groupToTrack);
                parameterSetDataItemCollector->groupToTrack = groupToTrack;
            }

            parameterSetDataItemCollector->method = Util::DataItemCollectorMethod::MEAN;
            if(XMLElement* methodElement = dataItemCollectorElement->FirstChildElement("method")){
                std::string method = string(methodElement->GetText());
                parameterSetDataItemCollector->method = Util::stringToDataItemCollectorMethod(method);
            }

            allParameterSetDataItemCollector.push_back(*parameterSetDataItemCollector);
        }
    }

    return allParameterSetDataItemCollector;
}