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

/** Creates a subdirectory with the name of newOutputLocation where all the various txt-files will be written.
 * \param newOutputLocation The path to the folder
 */
WriterTxt::WriterTxt(const string &outputLocation):
    Writer(),
    fileExtension(".txt")
{

    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    pid_t processID = getpid();

    this->outputLocation = "output/" + outputLocation + "_"+std::to_string(processID)+"_"+std::to_string(time);
    Util::createDirectory(this->outputLocation);


}

/** Standardconstructor
 */
WriterTxt::WriterTxt():WriterTxt("defaultOutputLocation") {


}


/** Destructor
 */
WriterTxt::~WriterTxt() = default;

void WriterTxt::vectorToFile(std::vector<double>* newVector, std::string name, int groupID) {
    name += "_Group";
    if(groupID==-1){
        name += "All";
    }
    else{
        name += to_string(groupID);
    }
    vectorToFile(newVector, name);
}

void WriterTxt::vectorToFile(std::vector<double>* newVector, std::string name, int groupID, Util::DataItemCollectorMethod method) {
    name += "_Group";
    if(groupID==-1){
        name += "All";
    }
    else{
        name += to_string(groupID);
    }
    name += "_";
    name += Util::dataItemCollectorMethodToString(method);
    vectorToFile(newVector, name);
}

void WriterTxt::vectorToFile(std::vector<double>* newVector, std::string name) {


	std::ofstream file((outputLocation+std::string("/")+name+fileExtension).c_str(), ios_base::app);
	file << std::scientific << std::setprecision(15);

    for (double i : *newVector) {
        file << i << "\n";
    }

	file.close();
}

void WriterTxt::matrixToFile(std::vector<std::vector<double>>* newVector, std::string name, int groupID) {
    name += "_Group";
    if(groupID==-1){
        name += "All";
    }
    else{
        name += to_string(groupID);
    }
    matrixToFile(newVector, name);
}

void WriterTxt::matrixToFile(std::vector<std::vector<double>>* newVector, std::string name, int groupID, Util::DataItemCollectorMethod method) {
    name += "_Group";
    if(groupID==-1){
        name += "All";
    }
    else{
        name += to_string(groupID);
    }
    name += "_";
    name += Util::dataItemCollectorMethodToString(method);
    matrixToFile(newVector, name);
}

void WriterTxt::matrixToFile(std::vector<std::vector<double>>* newVector, std::string name){

    std::ofstream file((outputLocation+std::string("/")+name+fileExtension).c_str(), ios_base::app);
    file << std::scientific << std::setprecision(15);

    for (auto &i : *newVector) {
        for(std::size_t j=0; j < i.size(); j++)
        {
            file << i.at(j) << ",";
        }
        file << "\n";
    }


    file.close();
}

void WriterTxt::arrayToFile(double* newArray, int arraySize, std::string name, int groupID) {
	// TODO: Implement if necessary
    boost::ignore_unused(newArray, arraySize, name, groupID);
	throw("Not yet implemented!");
}

void WriterTxt::saveInput(Parameter* newParameter){
	std::ofstream file((outputLocation+std::string("/input.csv")).c_str());
	file << std::scientific << std::setprecision(15);

    // General Parameters
    if(newParameter->outputname){
        file << "simulationName;" << *(newParameter->outputname) << ";" <<  "\n";
    }
    if(newParameter->writerClass){
        file << "writerClass;" << *(newParameter->writerClass) << ";" <<  "\n";
    }
    if(newParameter->numSteps){
        file << "numSteps;" << *(newParameter->numSteps) << ";" <<  "\n";
    }
    if(newParameter->deltaT){
        file << "deltaT;" << *(newParameter->deltaT) << ";" <<  "\n";
    }
    if(newParameter->startPrice){
        file << "startPrice;" << *(newParameter->startPrice) << ";" <<  "\n";
    }

    // ParametersetDividend
    if(newParameter->parameterSetDividend.Z1){
        file << "Z1;" << *(newParameter->parameterSetDividend.Z1) << ";" <<  "\n";
    }
    if(newParameter->parameterSetDividend.Z2){
        file << "Z2;" << *(newParameter->parameterSetDividend.Z2) << ";" <<  "\n";
    }
    if(newParameter->parameterSetDividend.interestRate){
        file << "interestRate;" << *(newParameter->parameterSetDividend.interestRate) << ";" <<  "\n";
    }
    if(newParameter->parameterSetDividend.initialDividend){
        file << "initialDividend;" << *(newParameter->parameterSetDividend.initialDividend) << ";" <<  "\n";
    }

    // ParameterSetRNG
    if(newParameter->parameterSetRNG.type){
        file << "randomGeneratorClass;" << *(newParameter->parameterSetRNG.type) << ";";
    }
    if(newParameter->parameterSetRNG.seed){
        file << "seed;" << *(newParameter->parameterSetRNG.seed) << ";";
    }
    if(newParameter->parameterSetRNG.poolSizeNormal){
        file << "poolSizeNormal;" << *(newParameter->parameterSetRNG.poolSizeNormal) << ";";
    }
    if(newParameter->parameterSetRNG.poolSizeUniform){
        file << "poolSizeUniform;" << *(newParameter->parameterSetRNG.poolSizeUniform) << ";";
    }
    if(newParameter->parameterSetRNG.enablePool){
        file << "enablePool;" << *(newParameter->parameterSetRNG.enablePool) << ";";
    }
    if(newParameter->parameterSetRNG.fileName){
        file << "fileName;" << *(newParameter->parameterSetRNG.fileName) << ";";
    }
    file <<  "\n";

    // ParameterSetPriceCalculator
    if(newParameter->parameterSetPriceCalculator.type){
        file << "priceCalculatorClass;" << *(newParameter->parameterSetPriceCalculator.type) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.epsilon){
        file << "epsilon;" << *(newParameter->parameterSetPriceCalculator.epsilon) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.maxIterations){
        file << "maxIterations;" << *(newParameter->parameterSetPriceCalculator.maxIterations) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.lowerBound){
        file << "lowerBound;" << *(newParameter->parameterSetPriceCalculator.lowerBound) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.upperBound){
        file << "upperBound;" << *(newParameter->parameterSetPriceCalculator.upperBound) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.theta){
        file << "theta;" << *(newParameter->parameterSetPriceCalculator.theta) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.fFunction){
        file << "fFunction;" << *(newParameter->parameterSetPriceCalculator.fFunction) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.fConstant){
        file << "fConstant;" << *(newParameter->parameterSetPriceCalculator.fConstant) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.gFunction){
        file << "gFunction;" << *(newParameter->parameterSetPriceCalculator.gFunction) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.gConstant){
        file << "gConstant;" << *(newParameter->parameterSetPriceCalculator.gConstant) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.marketDepth){
        file << "marketDepth;" << *(newParameter->parameterSetPriceCalculator.marketDepth) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.noiseFactor){
        file << "noiseFactor;" << *(newParameter->parameterSetPriceCalculator.noiseFactor) << ";";
    }
    if(newParameter->parameterSetPriceCalculator.mu){
        file << "mu;" << *(newParameter->parameterSetPriceCalculator.mu) << ";";
    }
    file <<  "\n";

    // ParameterSetAgent
    for (auto &i : newParameter->parameterSetAgent) {

        if(i.type){
            file << "agentClass;" << *(i.type) << ";";
        }
        if(i.count){
            file << "count;" << *(i.count) << ";";
        }
        if(i.cash){
            file << "cash;" << *(i.cash) << ";";
        }
        if(i.stock){
            file << "stock;" << *(i.stock) << ";";
        }
        if(i.C1){
            file << "C1;" << *(i.C1) << ";";
        }
        if(i.C2){
            file << "C2;" << *(i.C2) << ";";
        }
        if(i.C3){
            file << "C3;" << *(i.C3) << ";";
        }
        if(i.threshold){
            file << "threshold;" << *(i.threshold) << ";";
        }
        if(i.g){
            file << "g;" << *(i.g) << ";";
        }
        if(i.alpha){
            file << "alpha;" << *(i.alpha) << ";";
        }
        if(i.neighbourhoodGeneratorClass){
            file << "neighbourhoodGeneratorClass;" << *(i.neighbourhoodGeneratorClass) << ";";
        }
        if(i.A1){
            file << "A1;" << *(i.A1) << ";";
        }
        if(i.A2){
            file << "A2;" << *(i.A2) << ";";
        }
        if(i.b1){
            file << "b1;" << *(i.b1) << ";";
        }
        if(i.b2){
            file << "b2;" << *(i.b2) << ";";
        }
        if(i.gamma){
            file << "gamma;" << *(i.gamma) << ";";
        }
        if(i.stdNoiseSigma){
            file << "stdNoiseSigma;" << *(i.stdNoiseSigma) << ";";
        }
        if(i.riskTolerance){
            file << "riskTolerance;" << *(i.riskTolerance) << ";";
        }
        if(i.historyMean){
            file << "historyMean;" << *(i.historyMean) << ";";
        }
        if(i.historySigma){
            file << "historySigma;" << *(i.historySigma) << ";";
        }
        if(i.initialGamma){
            file << "initialGamma;" << *(i.initialGamma) << ";";
        }
        if(i.memorySpan){
            file << "memorySpan;" << *(i.memorySpan) << ";";
        }
        if(i.k){
            file << "k;" << *(i.k) << ";";
        }

        //FW
        if(i.eta){
            file << "eta;" << *(i.eta) << ";";
        }
        if(i.switchingStrategy){
            file << "switchingStrategy;" << *(i.switchingStrategy) << ";";
        }
        if(i.beta){
            file << "beta;" << *(i.beta) << ";";
        }
        if(i.indexStrategy){
            file << "indexStrategy;" << *(i.indexStrategy) << ";";
        }
        if(i.alpha_w){
            file << "alpha_w;" << *(i.alpha_w) << ";";
        }
        if(i.alpha_n){
            file << "alpha_n;" << *(i.alpha_n) << ";";
        }
        if(i.alpha_p){
            file << "alpha_p;" << *(i.alpha_p) << ";";
        }
        if(i.alpha_0){
            file << "alpha_0;" << *(i.alpha_0) << ";";
        }
        if(i.nu){
            file << "nu;" << *(i.nu) << ";";
        }
        if(i.sigma_c){
            file << "sigma_c;" << *(i.sigma_c) << ";";
        }
        if(i.chi){
            file << "chi;" << *(i.chi) << ";";
        }
        if(i.sigma_f){
            file << "sigma_f;" << *(i.sigma_f) << ";";
        }
        if(i.phi){
            file << "phi;" << *(i.phi) << ";";
        }
        if(i.fundamentalPrice){
            file << "fundamentalPrice;" << *(i.fundamentalPrice) << ";";
        }
        if(i.groups){
            file << "Groups;";
            for(auto &k : *(i.groups)){
                file << k << ";";
            }

        }

        file <<  "\n";
    }

    // ParameterSetDataItemCollector
    for (auto &i : newParameter->parameterSetDataItemCollector) {

        if(i.type){
            file << "dataItemCollectorClass;" << *(i.type) << ";";
        }
        if(i.writeInterval){
            file << "writeInterval;" << *(i.writeInterval) << ";";
        }
        if(i.collectInterval){
            file << "collectInterval;" << *(i.collectInterval) << ";";
        }
        if(i.method){
            file << "method;" << Util::dataItemCollectorMethodToString(*(i.method)) << ";";
        }
        if(i.groupToTrack){
            file << "groupToTrack;" << *(i.groupToTrack) << ";";
        }

        file <<  "\n";
    }

    // parameter set excess demamd calculator
    if(auto v = newParameter->parameterSetExcessDemandCalculator.excessDemandCalculatorClass){
        file << "excessDemandCalculatorClass" << *(v) << ";" <<  "\n";
    }


	file.close();
}

void WriterTxt::saveBuildInfo(){
    std::ofstream file((outputLocation+std::string("/buildInfo.csv")).c_str());

    file << "GIT_VERSION;" << buildinfo::GIT_VERSION << ";" <<  "\n";
    file << "GIT_SHA1" << buildinfo::GIT_SHA1 << ";" <<  "\n";
    file << "OS" << buildinfo::OS << ";" <<  "\n";
    file << "OS_NAME" << buildinfo::OS_NAME << ";" <<  "\n";
    file << "FQDN" << buildinfo::FQDN << ";" <<  "\n";
    file << "BUILD_TYPE" << buildinfo::BUILD_TYPE << ";" <<  "\n";
    file << "CXX" << buildinfo::CXX << ";" <<  "\n";

    file.close();
}

void WriterTxt::rngInformation(std::size_t uniformPoolInitialSize, std::size_t uniformPoolSize, int uniformPoolFills,
                               std::size_t normalPoolInitialSize, std::size_t normalPoolSize, int normalPoolFills, int seed){
    std::ofstream file((outputLocation+std::string("/usage.csv")).c_str(), ios_base::app);


    file << "rngInformation;" << 0 <<  ";" <<  "\n";
    file << "uniformPoolInitialSize;" << uniformPoolInitialSize << ";" <<  "\n";
    file << "uniformPoolUnused" << uniformPoolSize << ";" <<  "\n";
    file << "uniformPoolRefills" << uniformPoolFills << ";" <<  "\n";
    file << "normalPoolInitialSize" << normalPoolInitialSize << ";" <<  "\n";
    file << "normalPoolUnused" << normalPoolSize << ";" <<  "\n";
    file << "normalPoolRefills" << normalPoolFills << ";" <<  "\n";
    file << "seed" << seed << ";" <<  "\n";

    file.close();

}

void WriterTxt::saveTime(double time){
    std::ofstream file((outputLocation+std::string("/usage.csv")).c_str(), ios_base::app);

    file << "Time;" << 0 <<  ";" <<  "\n";
    file << "Simulation Time;" << time << ";" <<  "\n";

    file.close();
}
