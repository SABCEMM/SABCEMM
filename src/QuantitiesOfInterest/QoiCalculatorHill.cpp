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

#include "QoiCalculatorHill.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <cassert>

QoiCalculatorHill::QoiCalculatorHill() : QoiCalculatorHill(0) {}

QoiCalculatorHill::QoiCalculatorHill(double order) : order(order) {
    method = QuantityOfInterest::Method::hill;
    assert(order <= 1);
    assert(order > 0);
}

void QoiCalculatorHill::calculate(std::vector<std::vector<double>> *data){
    // Source : http://www.sfu.ca/~rgencay/evim.pdf (1.10)



    // hill estimator computed over timesteps:
    // case 1: data holds timestep many vectors with a value for each agent.
    // Assumption: Position of agentvalue in the vector is always the same (reasonable?)
    // Compute a hill estimator per agent
    if(data->size()>1){
        std::cerr << "QoiCalculatorHill: Multi-d case not implemented yet..." << std::endl;
        throw ("QoiCalculatorHill: Multi-d case not implemented yet...");
    }


    std::vector<double>& data_ = data->at(0);
    std::vector<double> temp;
    result.push_back(temp);

    if(!std::is_sorted(data_.begin(), data_.end())){
        std::sort(data_.begin(), data_.end());
    }
    if (data_.at(0) < 0){
        std::cerr << "QoiCalculatorHill: DataInput has Values < 0. Computation of Log not possible. 0 was returned."<< std::endl;
        result.at(0).push_back(0);
        return;
    }



    // Get the biggest order percent of values
    size_t order_ = size_t (ceil( (1-order) * data_.size() ));
    result.at(0).push_back(singleEstimator(order_, data_));

}

double QoiCalculatorHill::singleEstimator(size_t order_, std::vector<double>& data){
    //assuming sorted data
    assert(std::is_sorted(data.begin(), data.end()));
    double hill_estimator = 0;

    for (size_t i = order_; i < data.size(); i++){
        hill_estimator += log(data.at(i)) - log(data.at(order_));
    }
    hill_estimator = hill_estimator / order;
    return hill_estimator;

}

double QoiCalculatorHill::getParameter(){
    // Return ParameterValue for Naming if necessary
    return order;
}