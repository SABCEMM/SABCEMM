/*
 * DataItemCollectorAmountOfStock.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: cramer
 */

#include "DataItemCollectorAmountOfStock.h"
#include <string>
#include <cassert>


DataItemCollectorAmountOfStock::DataItemCollectorAmountOfStock() :
        DataItemCollectorAmountOfStock("mean") {

}

DataItemCollectorAmountOfStock::DataItemCollectorAmountOfStock(std::string method) {
	agents = nullptr;
	dataMatrix.clear();

	this->method = DataItemCollector::stringToDataItemCollectorMethod(method);
	if(this->method!=Method::DETAIL){
		std::vector<double> temp;
		dataMatrix.push_back(temp);
	}
}

DataItemCollectorAmountOfStock::~DataItemCollectorAmountOfStock() = default;

void DataItemCollectorAmountOfStock::collectData(){
	assert(agents != nullptr);

	std::vector<double> tempStock;

	// Loop over all Agents
	for (auto &agent : *agents) {
		// Check if Agent is part of tracked group.
		if(agent->hasGroup(groupToTrack_)){
			tempStock.push_back(agent->getStock());
		}
	}

	switch(method){
		case DataItemCollector::Method::MEAN:
			dataMatrix.at(0).push_back(Util::mean(tempStock));
			break;
		case DataItemCollector::Method::STD:
			dataMatrix.at(0).push_back(Util::std(tempStock));
			break;
		case DataItemCollector::Method::DETAIL:
			dataMatrix.push_back(tempStock);
            break;
        default:
            throw "invalid collector method" + DataItemCollector::methodToString(method);
	}
}


void DataItemCollectorAmountOfStock::checkInitilisation(){

	assert(agents!=nullptr);

}
void DataItemCollectorAmountOfStock::setAgents(std::vector<Agent*>* newAgents){

	agents = newAgents;

}

std::vector<std::vector<double>> * DataItemCollectorAmountOfStock::getData(){
	return &dataMatrix;
}