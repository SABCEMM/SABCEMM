/*
 * DataCollector.cpp
 *
 *  Created on: 01.05.2016
 *      Author: simon
 */

#include "DataCollector.h"
#include "../Parameter/Parameter.h"
#include "../Parameter/ParameterSetDataItemCollector.h"

#include "DataItemCollector.h"
#include "DataItemCollectorAmountOfCash.h"
#include "DataItemCollectorEMBGamma.h"
#include "DataItemCollectorExcessDemand.h"
#include "DataItemCollectorAmountOfStock.h"
#include "DataItemCollectorHarrasK.h"
#include "DataItemCollectorPrice.h"
#include "DataItemCollectorWealth.h"
#include "DataItemCollectorFWShares.h"

#include <boost/optional.hpp>

/** Standardconstructor of the DataCollector composite.
 */
DataCollector::DataCollector(){
	components.clear();
}


/** Destructor of the DataCollector composite.
 * Destroys all its components.
 */
DataCollector::~DataCollector() = default;

DataCollector* DataCollector::factory(Parameter* parameter, Price* price, ExcessDemand* excessDemand,
									  std::vector<Agent*>* agents, Writer* writer){
	//DataCollector
	DataCollector* dataCollector = new DataCollector();
	for (auto parameterSetDataItemCollector : parameter->parameterSetDataItemCollector) {
		optional<string> type = parameterSetDataItemCollector.type;

		DataItemCollector* tempDataCollector_;
		if (*type == "DataCollectorPrice")
		{
			DataItemCollectorPrice* tempDataCollector;
			tempDataCollector = new DataItemCollectorPrice(price);
			tempDataCollector->setName("Price");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorExcessDemand")
		{
			DataItemCollectorExcessDemand* tempDataCollector;
			tempDataCollector = new DataItemCollectorExcessDemand(excessDemand);
			tempDataCollector->setName("ExcessDemand");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorHarrasK")
		{
			DataItemCollectorHarrasK* tempDataCollector;
			tempDataCollector = new DataItemCollectorHarrasK(*(parameterSetDataItemCollector.method), agents);
			tempDataCollector->setName("HarrasK");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorEMBGamma")
		{
			DataItemCollectorEMBGamma* tempDataCollector;
			tempDataCollector = new DataItemCollectorEMBGamma();
			tempDataCollector->setAgents(agents);
			tempDataCollector->setName("EMBGamma");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorAmountOfCash")
		{
			DataItemCollectorAmountOfCash* tempDataCollector;
			tempDataCollector = new DataItemCollectorAmountOfCash(*(parameterSetDataItemCollector.method));
			tempDataCollector->setAgents(agents);
			tempDataCollector->setName("AmountOfCash");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorAmountOfStock")
		{
			DataItemCollectorAmountOfStock* tempDataCollector;
			tempDataCollector = new DataItemCollectorAmountOfStock(*(parameterSetDataItemCollector.method));
			tempDataCollector->setAgents(agents);
			tempDataCollector->setName("AmountOfStock");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorWealth")
		{
			DataItemCollectorWealth* tempDataCollector;
			tempDataCollector = new DataItemCollectorWealth(*(parameterSetDataItemCollector.method));
			tempDataCollector->setAgents(agents);
			tempDataCollector->setPrice(price);
			tempDataCollector->setName("Wealth");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}
		else if (*type == "DataCollectorFWShares")
		{
			DataItemCollectorFWShares* tempDataCollector;
			tempDataCollector = new DataItemCollectorFWShares();
			tempDataCollector->setAgents(agents);
			/// @todo maybe make this a class constant? That is similiar to the current implementation, but easier
			///       to maintain
			tempDataCollector->setName("FW-Share");
			tempDataCollector_ = static_cast<DataItemCollector *>(tempDataCollector);
		}

		else
		{
			throw("DataCollector Class unknown!");
		}
		//tempDataCollector_->setGroupToTrack(0);
		if(parameterSetDataItemCollector.collectInterval){
			tempDataCollector_->setCollectInterval(*(parameterSetDataItemCollector.collectInterval));
		}
		if(parameterSetDataItemCollector.writeInterval){
			tempDataCollector_->setWriteInterval(*(parameterSetDataItemCollector.writeInterval));
		}
		if(parameterSetDataItemCollector.groupToTrack){
			tempDataCollector_->setGroupToTrack(*(parameterSetDataItemCollector.groupToTrack));
		}
		tempDataCollector_->setWriter(writer);
		tempDataCollector_->checkInitilisation();
		dataCollector->add(tempDataCollector_);
	}

	return dataCollector;
}

/** Deletes all DataItemCollectors that have been given to the DataCollector.
 */
void DataCollector::deleteDataItemCollectors(){
	for (DataItemCollector* component : components) {
		delete component;
	}
	components.clear();
}

/** Passes the checkInitilisation to all components.
 */
void DataCollector::checkInitilisation(){
	for (auto &component : components) {
		component->checkInitilisation();
	}
}


/** Passes the write to all components.
 */
void DataCollector::write(){
	for (auto &component : components) {
		component->write();
	}
}


/** Passes the collect to all components.
 */
void DataCollector::collect(){
	for (auto &component : components) {
		component->collect();
	}
}


/** Adds a new component to the composite.
 * \param newDataCollector Pointer to a new DataCollector
 */
void DataCollector::add(DataItemCollector* newDataCollector){
	components.push_back(newDataCollector);
}



/** Passes the clearData to all components.
 */
void DataCollector::clearData(){
	for (auto &component : components) {
		component->clearData();
	}
}
