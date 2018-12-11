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
 * @file QuantitiesOfInterest.cpp
 * @author Cramer
 * @date 11 May 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <iostream>
#include "QuantitiesOfInterest.h"
#include "../Input/Input.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../Agent/Agent.h"
#include "../DataCollector/DataCollector.h"
#include "../DataCollector/DataItemCollectorPrice.h"
#include "../DataCollector/DataItemCollectorAmountOfCash.h"
#include "../DataCollector/DataItemCollectorEMBGamma.h"
#include "../DataCollector/DataItemCollectorExcessDemand.h"
#include "../DataCollector/DataItemCollectorAmountOfStock.h"
#include "../DataCollector/DataItemCollectorHarrasK.h"
#include "../DataCollector/DataItemCollectorPrice.h"
#include "../DataCollector/DataItemCollectorSwitchableShares.h"
#include "../DataCollector/DataItemCollectorWealth.h"
#include "../DataCollector/DataItemCollectorFWSharesFundamentalist.h"
#include "../DataCollector/DataItemCollectorFWSharesChartist.h"

QuantitiesOfInterest::QuantitiesOfInterest() {
    quantitiesOfInterest.clear();
}
QuantitiesOfInterest::~QuantitiesOfInterest() {
    for(auto& quantityOfInterest : quantitiesOfInterest){
        delete quantityOfInterest;
        quantityOfInterest = nullptr;
    }
    quantitiesOfInterest.clear();
}

QuantitiesOfInterest* QuantitiesOfInterest::factory(Input &input, Price *price, ExcessDemand *excessDemand,
                                      std::vector<Agent *> *agents,
                                      std::vector<Switchable *> &switchableGroups,
                                                    DataCollector* dataCollector){
    QuantitiesOfInterest *quantitiesOfInterest = new QuantitiesOfInterest();

    if(!input("QoI")) return quantitiesOfInterest;

    for (auto& QOIinput : input["QoI"].getChildren()) {
        std::string type = QOIinput.getName();
        if (QOIinput("quantity")){
            type = QOIinput["quantity"].getString();
        }

        DataItemCollector *tempDataCollector_ = nullptr;
        QuantityOfInterest* quantityOfInterest = nullptr;
        if (type == "price") {
            DataItemCollectorPrice *tempDataCollector;
            tempDataCollector = new DataItemCollectorPrice(price);
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);

            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "return" || type == "absreturn" || type == "logreturn" || type == "abslogreturn") {
            DataItemCollectorPrice *tempDataCollector;
            tempDataCollector = new DataItemCollectorPrice(price);
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);

            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        }
        else if (type == "excessdemand") {
            DataItemCollectorExcessDemand *tempDataCollector;
            tempDataCollector = new DataItemCollectorExcessDemand(excessDemand);
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "harrask") {
            DataItemCollectorHarrasK *tempDataCollector;
            tempDataCollector = new DataItemCollectorHarrasK(QOIinput("method") ? QOIinput["method"].getString() : "",
                                                             agents);

            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "embgamma") {
            DataItemCollectorEMBGamma *tempDataCollector;
            tempDataCollector = new DataItemCollectorEMBGamma(QOIinput("method") ? QOIinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);

            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "cash") {
            DataItemCollectorAmountOfCash *tempDataCollector;
            tempDataCollector = new DataItemCollectorAmountOfCash(
                    QOIinput("method") ? QOIinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);

            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "stock") {
            DataItemCollectorAmountOfStock *tempDataCollector;
            tempDataCollector = new DataItemCollectorAmountOfStock(
                    QOIinput("method") ? QOIinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);

            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "wealth") {
            DataItemCollectorWealth *tempDataCollector;
            tempDataCollector = new DataItemCollectorWealth(QOIinput("method") ? QOIinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);
            tempDataCollector->setPrice(price);

            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else if (type == "fwshares_fundamentalist") {
            DataItemCollectorFWSharesFundamentalist *tempDataCollector;
            tempDataCollector = new DataItemCollectorFWSharesFundamentalist();
            tempDataCollector->setAgents(agents);
            /// @todo maybe make this a class constant? That is similiar to the current implementation, but easier
            ///       to maintain
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        }else if (type == "fwshares_chartist") {
            DataItemCollectorFWSharesChartist *tempDataCollector;
            tempDataCollector = new DataItemCollectorFWSharesChartist();
            tempDataCollector->setAgents(agents);
            /// @todo maybe make this a class constant? That is similiar to the current implementation, but easier
            ///       to maintain
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        }else if (type == "switchableshares") {
            DataItemCollectorSwitchableShares *tempDataCollector;
            tempDataCollector = new DataItemCollectorSwitchableShares(switchableGroups);
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            quantityOfInterest = QuantityOfInterest::factory(tempDataCollector, QOIinput);
        } else {
            std::cerr << "Quantity Of Interest unknown!" << std::endl;
            throw ("Quantity Of Interest unknown!");
        }
        //tempDataCollector_->setGroupToTrack(0);

        if (QOIinput("groupToTrack")) {
            tempDataCollector_->setGroupToTrack(QOIinput["groupToTrack"].getInt());
            quantityOfInterest->setGroupToTrack(QOIinput["groupToTrack"].getInt());
        }

        quantityOfInterest->setName(QOIinput.getName());
        dataCollector->add(tempDataCollector_);
        quantitiesOfInterest->addQOI(quantityOfInterest);
    }

    return quantitiesOfInterest;
}


void QuantitiesOfInterest::addQOI(QuantityOfInterest* quantityOfInterest){
    quantitiesOfInterest.push_back(quantityOfInterest);
}

void QuantitiesOfInterest::calculateQoi(){
    for(auto qoi : quantitiesOfInterest){
        qoi->calculateQoI();
    }
}

void QuantitiesOfInterest::write(Writer* writer){
    for(auto qoi : quantitiesOfInterest){
        qoi->write(writer);
    }
}