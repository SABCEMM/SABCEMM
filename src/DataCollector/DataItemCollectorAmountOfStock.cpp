/*
 * DataItemCollectorAmountOfStock.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: cramer
 */

#include "DataItemCollectorAmountOfStock.h"


DataItemCollectorAmountOfStock::DataItemCollectorAmountOfStock() :
        DataItemCollectorAmountOfStock(Util::DataItemCollectorMethod::MEAN) {

}

DataItemCollectorAmountOfStock::DataItemCollectorAmountOfStock(Util::DataItemCollectorMethod method) {
	agents = nullptr;
	amountOfStock.clear();
	amountOfStockDetail.clear();

	this->method = method;
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
		case Util::DataItemCollectorMethod::MEAN:
			amountOfStock.push_back(Util::mean(tempStock));
			break;
		case Util::DataItemCollectorMethod::STD:
			amountOfStock.push_back(Util::std(tempStock));
			break;
		case Util::DataItemCollectorMethod::DETAIL:
			amountOfStockDetail.push_back(tempStock);
	}
}
void DataItemCollectorAmountOfStock::write(){

	assert(writer != nullptr);
	switch(method){
		case Util::DataItemCollectorMethod::DETAIL:
			writer->matrixToFile(&amountOfStockDetail, name, groupToTrack_, method);
			break;
		default:
			writer->vectorToFile(&amountOfStock, name, groupToTrack_, method);
	}

}
void DataItemCollectorAmountOfStock::clearData(){

	amountOfStock.clear();
	amountOfStockDetail.clear();

}
void DataItemCollectorAmountOfStock::checkInitilisation(){

	assert(agents!=nullptr);

}
void DataItemCollectorAmountOfStock::setAgents(std::vector<Agent*>* newAgents){

	agents = newAgents;

}
