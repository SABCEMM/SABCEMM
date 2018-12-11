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
 * @file QuantityOfInterest.h
 * @author Cramer
 * @date 11 May 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef PROJECT_QUANTITYOFINTEREST_H
#define PROJECT_QUANTITYOFINTEREST_H

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif


#include <map>
#include <string>
#include "../DataCollector/DataItemCollector.h"
#include "../Input/Input.h"
//#include "QoiCalculator.h"
class QoiCalculator;
//#include "../Writer/Writer.h"
class Writer;

class QuantityOfInterest {
#if BUILD_TESTS
    //FRIEND_TEST(**, **);
#endif
public:
    enum Method{
        full,
        mean,
        variance,
        kurtosis,
        excessKurtosis,
        moment,
        skew,
        minimum,
        maximum,
        quantile,
        hill,
        autocorrelation
    };
    enum Quantity{
        cash,
        stock,
        embGamma,
        excessDemand,
        fwSharesChartist,
        fwSharesFundamentalist,
        harrasK,
        price,
        switchableShares,
        wealth,
        return_,
        logReturn,
        absReturn,
        absLogReturn
    };

    static const std::map<std::string, QuantityOfInterest::Method> stringToMethod;
    static const std::map<QuantityOfInterest::Method, std::string> methodToString;
    static const std::map<std::string, QuantityOfInterest::Quantity> stringToQuantity;
    static const std::map<QuantityOfInterest::Quantity, std::string> quantityToString;

    QuantityOfInterest(QuantityOfInterest::Quantity quantity,
                       DataItemCollector *dataItemCollector);
    QuantityOfInterest(std::string quantity, DataItemCollector* dataItemCollector);
    void setCalculators(std::vector<QoiCalculator*> calculators);
    virtual ~QuantityOfInterest();
    virtual void calculateQoI();
    std::vector<double> getQoI();
    void write(Writer* writer);
    static QuantityOfInterest* factory(DataItemCollector* dataCollector, Input& QOIinput);
    void setName(const std::string &name);
    const std::string &getName() const;
    int getGroupToTrack() const;
    void setGroupToTrack(int groupToTrack);

protected:
    DataItemCollector* dataItemCollector;
    std::vector<QoiCalculator*> calculators;
    QuantityOfInterest::Quantity quantity;
    std::string name;
    int groupToTrack;

};


#endif //PROJECT_QUANTITYOFINTEREST_H
