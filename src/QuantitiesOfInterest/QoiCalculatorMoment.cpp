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

#include "QoiCalculatorMoment.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
using namespace boost::accumulators;

QoiCalculatorMoment::QoiCalculatorMoment() : QoiCalculatorMoment(1) {}

QoiCalculatorMoment::QoiCalculatorMoment(int moment) : moment_(moment) {
    method = QuantityOfInterest::Method::moment;
}

void QoiCalculatorMoment::calculate(std::vector<std::vector<double>> *data){
    // copy data since we are about to change it ...
    accumulator_set<double, stats<tag::mean> > acc;
    for(auto& v : *data){
        // copy data since we are about to change it ...
        std::vector<double> v_ = v;
        std::for_each( v_.begin(), v_.end(), [this](double &n){ std::pow(n, this->moment_); });
        acc = std::for_each( v_.begin(), v_.end(), acc );
    }
    std::vector<double> temp;
    result.push_back(temp);
    result.at(0).push_back(mean(acc));
}

double QoiCalculatorMoment::getParameter() {
    return moment_;
}
