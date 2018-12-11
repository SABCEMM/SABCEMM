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

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <boost/core/ignore_unused.hpp>
#include <chrono>

#include "WriterTxt.h"
#include "../Version/Version.h"
#include <cstddef> //for std::size_t

#include <tinyxml2/tinyxml2.h>

using namespace tinyxml2;
using namespace std;

/** Creates a subdirectory with the name of newOutputLocation where all the various txt-files will be written.
 * \param newOutputLocation The path to the folder
 */
WriterTxt::WriterTxt(const string &outputLocation):
    Writer()
{

    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();

    this->outputLocation = std::string("./output/")+outputLocation+"_"+std::to_string(time)+std::string(".xml");

    doc = new XMLDocument();
    root = doc->NewElement("results");
    doc->InsertFirstChild(root);

    simulationNumber = 0;

}

/** Standardconstructor
 */
WriterTxt::WriterTxt():WriterTxt("defaultOutputLocation") {
}


/** Destructor
 */
WriterTxt::~WriterTxt(){
    XMLError eResult = doc->SaveFile(outputLocation.c_str(), false);
    XMLCheckResult(eResult);
    delete doc;
    doc = nullptr;
}

void WriterTxt::XMLCheckResult(XMLError xmlError){
        if (xmlError != XML_SUCCESS)
        {
            std::cerr << "Simulations in " << outputLocation << " did not save to disk! Error: " << xmlError << std::endl;
        }
}


void WriterTxt::saveInput(Input& input){
    XMLElement* inputRootElem = simulationElem->FirstChildElement("input");

    for (auto& inputElem : input.getChildren()){
        parse(inputElem, inputRootElem);
    }

}

void WriterTxt::parse(Input &input, XMLElement *elem) {
    if(!input.hasChildren()){
        XMLElement* XMLchild = doc->NewElement(input.getName().c_str());
        if (input.hasValue()){
            XMLchild->SetText(input.getString().c_str());
        }
        elem->InsertEndChild(XMLchild);
    } else{
        XMLElement* XMLchild = doc->NewElement(input.getName().c_str());
        elem->InsertEndChild(XMLchild);
        for(auto& inputChild : input.getChildren()){
            parse(inputChild, XMLchild);
        }
    }
}


void WriterTxt::saveBuildInfo(){
    XMLElement* infoElem = nullptr;
    XMLNode* buildInfoElem = simulationElem->FirstChildElement("buildInfo");


    infoElem = doc->NewElement("GIT_VERSION");
    infoElem->SetText(buildinfo::GIT_VERSION.c_str());
    buildInfoElem->InsertEndChild(infoElem);

    infoElem = doc->NewElement("GIT_SHA1");
    infoElem->SetText(buildinfo::GIT_SHA1.c_str());
    buildInfoElem->InsertEndChild(infoElem);

    infoElem = doc->NewElement("OS");
    infoElem->SetText(buildinfo::OS.c_str());
    buildInfoElem->InsertEndChild(infoElem);

    infoElem = doc->NewElement("OS_NAME");
    infoElem->SetText(buildinfo::OS_NAME.c_str());
    buildInfoElem->InsertEndChild(infoElem);

    infoElem = doc->NewElement("FQDN");
    infoElem->SetText(buildinfo::FQDN.c_str());
    buildInfoElem->InsertEndChild(infoElem);

    infoElem = doc->NewElement("BUILD_TYPE");
    infoElem->SetText(buildinfo::BUILD_TYPE.c_str());
    buildInfoElem->InsertEndChild(infoElem);

    infoElem = doc->NewElement("CXX");
    infoElem->SetText(buildinfo::CXX.c_str());
    buildInfoElem->InsertEndChild(infoElem);

}

void WriterTxt::rngInformation(std::size_t &uniformGenerated, std::size_t &uniformUnused, std::size_t &normalGenerated,
                               std::size_t &normalUnused, int &seed) {
    XMLNode* usageElem = simulationElem->FirstChildElement("usage");
    XMLNode* rngInformationElem = doc->NewElement("rngInformation");
    usageElem->InsertEndChild(rngInformationElem);

    XMLElement* uniformGeneratedElem = doc->NewElement("uniformGenerated");
    uniformGeneratedElem->SetText((int)uniformGenerated);
    rngInformationElem->InsertEndChild(uniformGeneratedElem);

    XMLElement* uniformUnusedElem = doc->NewElement("uniformUnused");
    uniformUnusedElem->SetText((int)uniformUnused);
    rngInformationElem->InsertEndChild(uniformUnusedElem);

    XMLElement* normalGeneratedElem = doc->NewElement("normalGenerated");
    normalGeneratedElem->SetText((int)normalGenerated);
    rngInformationElem->InsertEndChild(normalGeneratedElem);

    XMLElement* normalUnusedElem = doc->NewElement("normalUnused");
    normalUnusedElem->SetText((int)normalUnused);
    rngInformationElem->InsertEndChild(normalUnusedElem);

    XMLElement* seedElem = doc->NewElement("seed");
    seedElem->SetText((int)seed);
    rngInformationElem->InsertEndChild(seedElem);

}

void WriterTxt::saveTime(double time){
    XMLNode* usageElem = simulationElem->FirstChildElement("usage");

    XMLElement* timeElem = doc->NewElement("SimulationTime");
    timeElem->SetText(time);
    usageElem->InsertEndChild(timeElem);
}


void WriterTxt::addSimulation(std::string simulationIdentifier) {
    simulationIdentifier = simulationIdentifier + "_" + std::to_string(simulationNumber);
    simulationNumber++;
    simulationElem = doc->NewElement(simulationIdentifier.c_str());
    root->InsertEndChild(simulationElem);

    XMLNode* output = doc->NewElement("output");
    simulationElem->InsertEndChild(output);
    XMLNode* usage = doc->NewElement("usage");
    simulationElem->InsertEndChild(usage);
    XMLNode* buildInfo = doc->NewElement("buildInfo");
    simulationElem->InsertEndChild(buildInfo);
    XMLNode* input = doc->NewElement("input");
    simulationElem->InsertEndChild(input);
}


void WriterTxt::addQoI(string method, QuantityOfInterest::Quantity quantity, int groupID,
                       std::vector<std::vector<double>> *newVector, string &name_) {
    XMLNode* outputElem = simulationElem->FirstChildElement("output");

    XMLElement* qoiElem = nullptr;
    qoiElem = outputElem->FirstChildElement(name_.c_str());
    if(qoiElem == nullptr){
        qoiElem = doc->NewElement(name_.c_str());
        qoiElem->SetAttribute("GroupToTrack", groupID);
        qoiElem->SetAttribute("Quantity", QuantityOfInterest::quantityToString.at(quantity).c_str());
        outputElem->InsertEndChild(qoiElem);
    }

    XMLElement* methodElem = doc->NewElement(method.c_str());
    qoiElem->InsertEndChild(methodElem);

    for(std::size_t i = 0; i<newVector->size(); i++){
        std::string row_name = std::string("r_")+std::to_string(i);
        XMLNode* rowElem = doc->NewElement(row_name.c_str());
        methodElem->InsertEndChild(rowElem);
        for(std::size_t j = 0; j < newVector->at(i).size(); j++){
            std::string column_name = std::string("c_")+std::to_string(i);
            XMLElement* dataElem = doc->NewElement(column_name.c_str());
            dataElem->SetText(newVector->at(i).at(j));
            rowElem->InsertEndChild(dataElem);
        }
    }
}