/*
 * DataCollector.cpp
 *
 *  Created on: 01.05.2016
 *      Author: simon
 */

#include "DataCollector.h"
#include <iostream>

#include "DataItemCollectorAmountOfCash.h"
#include "DataItemCollectorEMBGamma.h"
#include "DataItemCollectorLLSMemorySpans.h"
#include "DataItemCollectorExcessDemand.h"
#include "DataItemCollectorAmountOfStock.h"
#include "DataItemCollectorHarrasK.h"
#include "DataItemCollectorPrice.h"
#include "DataItemCollectorSwitchableShares.h"
#include "DataItemCollectorWealth.h"
#include "DataItemCollectorFWSharesFundamentalist.h"
#include "DataItemCollectorFWSharesChartist.h"
#include <string>

/** Standardconstructor of the DataCollector composite.
 */
DataCollector::DataCollector() {
    components.clear();
}


/** Destructor of the DataCollector composite.
 * Destroys all its components.
 */
DataCollector::~DataCollector(){
    deleteDataItemCollectors();
};

DataCollector *DataCollector::factory(Input &input, Price *price, ExcessDemand *excessDemand,
                                      std::vector<Agent *> *agents,
                                      std::vector<Switchable *> &switchableGroups) {

    //DataCollector
    DataCollector *dataCollector = new DataCollector();
    if(!input("dataItemCollectorClasses")){
        return dataCollector;
    }
    for (auto &DICinput : input["dataItemCollectorClasses"].getChildren()) {
        std::string type = DICinput.getName();

        DataItemCollector *tempDataCollector_;
        if (type == "datacollectorprice") {
            DataItemCollectorPrice *tempDataCollector;
            tempDataCollector = new DataItemCollectorPrice(price);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "Price");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectorexcessdemand") {
            DataItemCollectorExcessDemand *tempDataCollector;
            tempDataCollector = new DataItemCollectorExcessDemand(excessDemand);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "ExcessDemand");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectorharrask") {
            DataItemCollectorHarrasK *tempDataCollector;
            tempDataCollector = new DataItemCollectorHarrasK(DICinput("method") ? DICinput["method"].getString() : "",
                                                             agents);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "HarrasK");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectorembgamma") {
            DataItemCollectorEMBGamma *tempDataCollector;
            tempDataCollector = new DataItemCollectorEMBGamma(DICinput("method") ? DICinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "EMBGamma");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectorllsmemoryspans") {
            DataItemCollectorLLSMemorySpans *tempDataCollector;
            tempDataCollector = new DataItemCollectorLLSMemorySpans;
            tempDataCollector->setAgents(agents);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "LLSMemorySpans");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);

        } else if (type == "datacollectoramountofcash") {
            DataItemCollectorAmountOfCash *tempDataCollector;
            tempDataCollector = new DataItemCollectorAmountOfCash(
                    DICinput("method") ? DICinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "AmountOfCash");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectoramountofstock") {
            DataItemCollectorAmountOfStock *tempDataCollector;
            tempDataCollector = new DataItemCollectorAmountOfStock(
                    DICinput("method") ? DICinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "AmountOfStock");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectorwealth") {
            DataItemCollectorWealth *tempDataCollector;
            tempDataCollector = new DataItemCollectorWealth(DICinput("method") ? DICinput["method"].getString() : "");
            tempDataCollector->setAgents(agents);
            tempDataCollector->setPrice(price);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "Wealth");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else if (type == "datacollectorfwshares") {
            DataItemCollectorFWSharesFundamentalist *tempDataCollector;
            tempDataCollector = new DataItemCollectorFWSharesFundamentalist();
            tempDataCollector->setAgents(agents);
            /// @todo maybe make this a class constant? That is similiar to the current implementation, but easier
            ///       to maintain
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString()+"_Fundamentalist" : "FW-Share_Fundamentalist");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
            // TODO: Copied Code ... Make nice once in a while ...
            if (DICinput("collectInterval")) {
                tempDataCollector_->setCollectInterval(DICinput["collectInterval"].getInt());
            }
            if (DICinput("groupToTrack")) {
                tempDataCollector_->setGroupToTrack(DICinput["groupToTrack"].getInt());
            }
            tempDataCollector_->checkInitilisation();
            dataCollector->add(tempDataCollector_);

            DataItemCollectorFWSharesChartist *tempDataCollector2;
            tempDataCollector2 = new DataItemCollectorFWSharesChartist();
            tempDataCollector2->setAgents(agents);
            /// @todo maybe make this a class constant? That is similiar to the current implementation, but easier
            ///       to maintain
            tempDataCollector2->setName(DICinput("name") ? DICinput["name"].getString()+"_Chartist" : "FW-Share_Chartist");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector2);
        } else if (type == "datacollectorswitchableshares") {
            DataItemCollectorSwitchableShares *tempDataCollector;
            tempDataCollector = new DataItemCollectorSwitchableShares(switchableGroups);
            tempDataCollector->setName(DICinput("name") ? DICinput["name"].getString() : "Charist-Share");
            tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
        } else {
            throw ("datacollector Class unknown!");
        }
        //tempDataCollector_->setGroupToTrack(0);
        if (DICinput("collectInterval")) {
            tempDataCollector_->setCollectInterval(DICinput["collectInterval"].getInt());
        }
        if (DICinput("groupToTrack")) {
            tempDataCollector_->setGroupToTrack(DICinput["groupToTrack"].getInt());
        }
        tempDataCollector_->checkInitilisation();
        dataCollector->add(tempDataCollector_);
    }

    return dataCollector;
}

/** Deletes all DataItemCollectors that have been given to the DataCollector.
 */
void DataCollector::deleteDataItemCollectors() {
    for (auto component : components) {
        delete component;
    }
    components.clear();
}

/** Passes the checkInitilisation to all components.
 */
void DataCollector::checkInitilisation() {
    for (auto &component : components) {
        component->checkInitilisation();
    }
}



/** Passes the collect to all components.
 */
void DataCollector::collect() {
    for (auto &component : components) {
        component->collect();
    }
}


/** Adds a new component to the composite.
 * \param newDataCollector Pointer to a new DataCollector
 */
void DataCollector::add(DataItemCollector *newDataCollector) {
    components.push_back(newDataCollector);
}


/** Passes the clearData to all components.
 */
void DataCollector::clearData() {
    for (auto component : components) {
        component->clearData();
    }
}