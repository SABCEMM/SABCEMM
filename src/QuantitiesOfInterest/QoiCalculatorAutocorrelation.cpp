

/* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Cramer, Simon
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
 * @file QuantityOfInterest.cpp
 * @author Cramer
 * @date 11 May 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "QoiCalculatorAutocorrelation.h"
#include "QoiCalculatorMean.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <cassert>

QoiCalculatorAutocorrelation::QoiCalculatorAutocorrelation() : QoiCalculatorAutocorrelation(2) {}

QoiCalculatorAutocorrelation::QoiCalculatorAutocorrelation(size_t tau) : tau(tau) {
    method = QuantityOfInterest::Method::autocorrelation;
    if (this->tau < 2)
        this->tau = 2;
}

void QoiCalculatorAutocorrelation::calculate(std::vector<std::vector<double>> *data){
    // Source : https://de.wikipedia.org/wiki/Autokorrelation#Schätzung



    // autocorrelation estimator computed over timesteps:
    // case 1: data holds timestep many vectors with a value for each agent.
    // Assumption: Position of agentvalue in the vector is always the same (reasonable?)
    // Compute a autocorrelation estimator per agent
    if(data->size()>1){
        std::cerr << "QoiCalculatorAutocorrelation: Multi-d case not implemented yet..." << std::endl;
        throw ("QoiCalculatorAutocorrelation: Multi-d case not implemented yet...");
    }


    std::vector<double>& data_ = data->at(0);

    if(tau > data_.size()){
        tau = data_.size();
    }

    std::vector<double> temp;
    result.push_back(temp);

    QoiCalculatorMean qoiCalculatorMean;
    qoiCalculatorMean.calculate(data);
    double mean = qoiCalculatorMean.getResult()->at(0).at(0);

    double gamma_0 = 0;
    for(size_t i = 0; i<data_.size(); i++){
        gamma_0 += pow(data_.at(i)-mean,2);
    }


    result.at(0).push_back(singleEstimator(tau, data_, gamma_0, mean));
}

double
QoiCalculatorAutocorrelation::singleEstimator(size_t tau_, std::vector<double> &data, double &gamma_0, double &mean) {
    //assuming sorted data
    double autocorr = 0;

    for (size_t i = 0; i < data.size()-tau_; i++){
        autocorr += (data.at(i+tau_) - mean)*(data.at(i)-mean) / gamma_0;
    }
    return autocorr;

}

double QoiCalculatorAutocorrelation::getParameter() {
    return tau;
}