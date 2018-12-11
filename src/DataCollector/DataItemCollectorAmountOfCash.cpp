/*
 * DataItemCollectorAmountOfCash.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: cramer
 */
#include <vector>
#include <string>

#include "../Agent/Agent.h"
#include "DataItemCollectorAmountOfCash.h"
#include <cassert>


DataItemCollectorAmountOfCash::DataItemCollectorAmountOfCash() :
        DataItemCollectorAmountOfCash("mean") {

}

DataItemCollectorAmountOfCash::DataItemCollectorAmountOfCash(DataItemCollector::Method method): method(method){
    agents = nullptr;
    dataMatrix.clear();
    if(method!=Method::DETAIL){
        std::vector<double> temp;
        dataMatrix.push_back(temp);
    }
}

DataItemCollectorAmountOfCash::DataItemCollectorAmountOfCash(std::string method) : DataItemCollectorAmountOfCash(DataItemCollector::stringToDataItemCollectorMethod(method)) {
}

DataItemCollectorAmountOfCash::~DataItemCollectorAmountOfCash() = default;

void DataItemCollectorAmountOfCash::collectData(){

	assert(agents != nullptr);

	std::vector<double> tempCash;

	// Loop over all Agents
	for (auto &agent : *agents) {
		// Check if Agent is part of tracked group.
		if(agent->hasGroup(groupToTrack_)){

            tempCash.push_back(agent->getCash());
		}
	}

    if(tempCash.empty())
    {
        dataMatrix.at(0).push_back(0);
        return;
    }

    switch(method){
        case DataItemCollector::Method::MEAN:
            dataMatrix.at(0).push_back(Util::mean(tempCash));
            break;
        case DataItemCollector::Method::STD:
            dataMatrix.at(0).push_back(Util::std(tempCash));
            break;
        case DataItemCollector::Method::DETAIL:
            dataMatrix.push_back(tempCash);
            break;
        default:
            throw "invalid collector method " + DataItemCollector::methodToString(method);
    }
}


void DataItemCollectorAmountOfCash::checkInitilisation(){

	assert(agents!=nullptr);

}

void DataItemCollectorAmountOfCash::setAgents(std::vector<Agent*>* newAgents){

	agents = newAgents;

}

std::vector<std::vector<double>> * DataItemCollectorAmountOfCash::getData(){
    return &dataMatrix;
}