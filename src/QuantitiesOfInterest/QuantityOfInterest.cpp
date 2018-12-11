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

#include "QuantityOfInterest.h"
#include "QoiCalculatorExcessKurtosis.h"
#include "QoiCalculatorKurtosis.h"
#include "QoiCalculatorMaximum.h"
#include "QoiCalculatorMean.h"
#include "QoiCalculatorMinimum.h"
#include "QoiCalculatorMoment.h"
#include "QoiCalculatorQuantile.h"
#include "QoiCalculatorSkew.h"
#include "QoiCalculatorVariance.h"
#include "QoiCalculatorFull.h"
#include "../Writer/Writer.h"
#include "QoiCalculatorHill.h"
#include "QoiCalculatorAutocorrelation.h"

#include <iostream>

#include <boost/assign.hpp>

using namespace std;

const std::map<std::string, QuantityOfInterest::Method> QuantityOfInterest::stringToMethod =
        boost::assign::map_list_of
                ("full", QuantityOfInterest::Method::full)
                ("mean", QuantityOfInterest::Method::mean)
                ("variance", QuantityOfInterest::Method::variance)
                ("kurtosis", QuantityOfInterest::Method::kurtosis)
                ("excesskurtosis", QuantityOfInterest::Method::excessKurtosis)
                ("moment", QuantityOfInterest::Method::moment)
                ("skew", QuantityOfInterest::Method::skew)
                ("minimum", QuantityOfInterest::Method::minimum)
                ("maximum", QuantityOfInterest::Method::maximum)
                ("quantile", QuantityOfInterest::Method::quantile)
                ("hill", QuantityOfInterest::Method::hill)
                ("autocorrelation", QuantityOfInterest::Method::autocorrelation);
const std::map<QuantityOfInterest::Method, std::string> QuantityOfInterest::methodToString =
        boost::assign::map_list_of
                (QuantityOfInterest::Method::full, "full")
                (QuantityOfInterest::Method::mean, "mean")
                (QuantityOfInterest::Method::variance, "variance")
                (QuantityOfInterest::Method::kurtosis, "kurtosis")
                (QuantityOfInterest::Method::excessKurtosis, "excesskurtosis")
                (QuantityOfInterest::Method::moment, "moment")
                (QuantityOfInterest::Method::skew, "skew")
                (QuantityOfInterest::Method::minimum, "minimum")
                (QuantityOfInterest::Method::maximum, "maximum")
                (QuantityOfInterest::Method::quantile, "quantile")
                (QuantityOfInterest::Method::hill, "hill")
                (QuantityOfInterest::Method::autocorrelation, "autocorrelation");
const std::map<std::string, QuantityOfInterest::Quantity> QuantityOfInterest::stringToQuantity =
        boost::assign::map_list_of
                ("cash", QuantityOfInterest::Quantity::cash)
                ("stock", QuantityOfInterest::Quantity::stock)
                ("embgamma", QuantityOfInterest::Quantity::embGamma)
                ("excessdemand", QuantityOfInterest::Quantity::excessDemand)
                ("fwshares_chartist", QuantityOfInterest::Quantity::fwSharesChartist)
                ("fwshares_fundamentalist", QuantityOfInterest::Quantity::fwSharesFundamentalist)
                ("harrask", QuantityOfInterest::Quantity::harrasK)
                ("price", QuantityOfInterest::Quantity::price)
                ("switchableshares", QuantityOfInterest::Quantity::switchableShares)
                ("wealth", QuantityOfInterest::Quantity::wealth)
                ("return", QuantityOfInterest::Quantity::return_)
                ("absreturn", QuantityOfInterest::Quantity::absReturn)
                ("logreturn", QuantityOfInterest::Quantity::logReturn)
                ("abslogreturn", QuantityOfInterest::Quantity::absLogReturn);

const std::map<QuantityOfInterest::Quantity, std::string> QuantityOfInterest::quantityToString =
        boost::assign::map_list_of
                (QuantityOfInterest::Quantity::cash, "cash")
                (QuantityOfInterest::Quantity::stock, "stock")
                (QuantityOfInterest::Quantity::embGamma, "embgamma")
                (QuantityOfInterest::Quantity::excessDemand, "excessdemand")
                (QuantityOfInterest::Quantity::fwSharesChartist, "fwshares_chartist")
                (QuantityOfInterest::Quantity::fwSharesFundamentalist, "fwshares_fundamentalist")
                (QuantityOfInterest::Quantity::harrasK, "harrask")
                (QuantityOfInterest::Quantity::price, "price")
                (QuantityOfInterest::Quantity::switchableShares, "switchableshares")
                (QuantityOfInterest::Quantity::wealth, "wealth")
                (QuantityOfInterest::Quantity::return_, "return")
                (QuantityOfInterest::Quantity::absReturn, "absreturn")
                (QuantityOfInterest::Quantity::logReturn, "logreturn")
                (QuantityOfInterest::Quantity::absLogReturn, "abslogreturn");

QuantityOfInterest::QuantityOfInterest(QuantityOfInterest::Quantity quantity,
                                       DataItemCollector *dataItemCollector) :
        dataItemCollector(dataItemCollector), quantity(quantity) {
    calculators.clear();
    groupToTrack = -1;
    name = "NONE";
}

QuantityOfInterest::QuantityOfInterest(std::string quantity,
                                       DataItemCollector *dataItemCollector) :
        QuantityOfInterest(
                QuantityOfInterest::stringToQuantity.at(quantity),
                dataItemCollector) {

}

QuantityOfInterest::~QuantityOfInterest() {
    for (auto& calculator: calculators) {
        delete calculator;
        calculator = nullptr;
    }
    calculators.clear();
}

QuantityOfInterest *QuantityOfInterest::factory(DataItemCollector *dataItemCollector, Input &QOIinput) {
    std::string type = QOIinput.getName();
    std::string name_ = QOIinput.getName();
    if (QOIinput("quantity")){
        type = QOIinput["quantity"].getString();
    }

    QuantityOfInterest::Quantity quantity;
    try {
        quantity = QuantityOfInterest::stringToQuantity.at(type);
    }
    catch (std::out_of_range &e) {
        throw("QoI unknown!");
    }

    QuantityOfInterest *quantityOfInterest = new QuantityOfInterest(type, dataItemCollector);

    quantityOfInterest->setName(name_);
    std::vector<QoiCalculator *> calculators;
    for (auto& method_: QOIinput.getChildren()) {
        QuantityOfInterest::Method method;
        try {
            method = QuantityOfInterest::stringToMethod.at(method_.getName());
        }
        catch (std::out_of_range &e) {
            continue;
        }

        QoiCalculator *calculator = nullptr;
        switch (method) {
            case QuantityOfInterest::Method::mean:
                calculator = new QoiCalculatorMean();
                break;
            case QuantityOfInterest::Method::variance:
                calculator = new QoiCalculatorVariance();
                break;
            case QuantityOfInterest::Method::kurtosis:
                calculator = new QoiCalculatorKurtosis();
                break;
            case QuantityOfInterest::Method::excessKurtosis:
                calculator = new QoiCalculatorExcessKurtosis();
                break;
            case QuantityOfInterest::Method::moment:
                if(method_.hasChildren()){
                    for(auto& m: method_.getChildren()){
                        calculator = new QoiCalculatorMoment(m.getInt());
                        calculators.push_back(calculator);
                    }
                    continue;
                }
                else{
                    calculator = new QoiCalculatorMoment(method_.getInt());
                }
                break;
            case QuantityOfInterest::Method::skew:
                calculator = new QoiCalculatorSkew();
                break;
            case QuantityOfInterest::Method::minimum:
                calculator = new QoiCalculatorMinimum();
                break;
            case QuantityOfInterest::Method::maximum:
                calculator = new QoiCalculatorMaximum();
                break;
            case QuantityOfInterest::Method::quantile:
                calculator = new QoiCalculatorQuantile();
                break;
            case QuantityOfInterest::Method::full:
                calculator = new QoiCalculatorFull();
                break;
            case QuantityOfInterest::Method::hill:
                if(method_.hasChildren()){
                    for(auto& m: method_.getChildren()){
                        calculator = new QoiCalculatorHill(m.getDouble());
                        calculators.push_back(calculator);
                    }
                    continue;
                }
                else {
                    calculator = new QoiCalculatorHill(method_.getDouble());
                }
                break;
            case QuantityOfInterest::Method::autocorrelation:
                if(method_.hasChildren()){
                    for(auto& m: method_.getChildren()){
                        calculator = new QoiCalculatorAutocorrelation(m.getSizeT());
                        calculators.push_back(calculator);
                    }
                    continue;
                }
                else {
                    calculator = new QoiCalculatorAutocorrelation(method_.getSizeT());
                }
                break;
            default:
                cerr << "Quantity Of Interest " << QuantityOfInterest::methodToString.at(method)
                     << " not implemented yet. It's ignored." << endl;
                break;
        }
        calculators.push_back(calculator);

    }

    quantityOfInterest->setCalculators(calculators);

    return quantityOfInterest;
}

void QuantityOfInterest::setCalculators(std::vector<QoiCalculator *> calculators) {
    this->calculators = calculators;
}

void QuantityOfInterest::calculateQoI() {
    std::vector<std::vector<double>>* data = dataItemCollector->getData();

    switch (quantity) {
        case QuantityOfInterest::Quantity::logReturn:
            assert(data->size() == 1);
            assert(data->at(0).size() > 1);

            for(size_t i = 0; i < data->at(0).size()-1; i++){
                // equivalent to log(data->at(0).at(i+1)) - log(data->at(0).at(i) ) but only one expensive logarithm needs to be computed
                data->at(0).at(i) = log(data->at(0).at(i+1) / data->at(0).at(i) );
            }

            // Delete last element. We can't compute return here because we are missing data.
            data->at(0).pop_back();
            break;
        case QuantityOfInterest::Quantity::absLogReturn:
            assert(data->size() == 1);
            assert(data->at(0).size() > 1);

            for(size_t i = 0; i < data->at(0).size()-1; i++){
                data->at(0).at(i) = abs( log( data->at(0).at(i+1) / data->at(0).at(i) ) );
            }

            // Delete last element. We can't compute return here because we are missing data.
            data->at(0).pop_back();
            break;
        case QuantityOfInterest::Quantity::return_:
            assert(data->size() == 1);
            assert(data->at(0).size() > 1);

            for(size_t i = 0; i < data->at(0).size()-1; i++){
                data->at(0).at(i) = ( data->at(0).at(i+1) / data->at(0).at(i) ) -1 ;
            }

            // Delete last element. We can't compute return here because we are missing data.
            data->at(0).pop_back();
            break;
        case QuantityOfInterest::Quantity::absReturn:
            assert(data->size() == 1);
            assert(data->at(0).size() > 1);

            for(size_t i = 0; i < data->at(0).size()-1; i++){
                data->at(0).at(i) = abs( data->at(0).at(i+1) / data->at(0).at(i) -1 );
            }

            // Delete last element. We can't compute return here because we are missing data.
            data->at(0).pop_back();
            break;
        default:
            assert(data->size() > 0);
            assert(data->at(0).size() > 0);

    }

    for (auto calculator : calculators) {
        calculator->calculate(data);
    }
    dataItemCollector->clearData();
}

void QuantityOfInterest::write(Writer* writer){
    for(auto calculator : calculators){
        std::string method_name = QuantityOfInterest::methodToString.at(calculator->getMethod());

        if (calculator->getMethod() == QuantityOfInterest::Method::hill ||
            calculator->getMethod() == QuantityOfInterest::Method::autocorrelation ||
            calculator->getMethod() == QuantityOfInterest::Method::moment){
            method_name += "_"+std::to_string(calculator->getParameter());
        }
        writer->addQoI(method_name, quantity, groupToTrack,
                calculator->getResult(), this->name);
    }
}

const string &QuantityOfInterest::getName() const {
    return name;
}

void QuantityOfInterest::setName(const string &name) {
    this->name = name;
}

int QuantityOfInterest::getGroupToTrack() const {
    return groupToTrack;
}

void QuantityOfInterest::setGroupToTrack(int groupToTrack) {
    QuantityOfInterest::groupToTrack = groupToTrack;
}
