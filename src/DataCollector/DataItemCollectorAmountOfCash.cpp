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


DataItemCollectorAmountOfCash::DataItemCollectorAmountOfCash() :
        DataItemCollectorAmountOfCash(Util::DataItemCollectorMethod::MEAN) {

}

DataItemCollectorAmountOfCash::DataItemCollectorAmountOfCash(Util::DataItemCollectorMethod method) {
	agents = nullptr;
	amountOfCash.clear();
	amountOfCashDetail.clear();

	this->method = method;

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

    switch(method){
        case Util::DataItemCollectorMethod::MEAN:
            amountOfCash.push_back(Util::mean(tempCash));
            break;
        case Util::DataItemCollectorMethod::STD:
            amountOfCash.push_back(Util::std(tempCash));
            break;
        case Util::DataItemCollectorMethod::DETAIL:
            amountOfCashDetail.push_back(tempCash);
    }



}
void DataItemCollectorAmountOfCash::write(){
	assert(writer != nullptr);

    switch(method){
        case Util::DataItemCollectorMethod::DETAIL:
            writer->matrixToFile(&amountOfCashDetail, name, groupToTrack_, method);
            break;
        default:
            writer->vectorToFile(&amountOfCash, name, groupToTrack_, method);
    }
}
void DataItemCollectorAmountOfCash::clearData(){

	amountOfCash.clear();
    amountOfCashDetail.clear();

}
void DataItemCollectorAmountOfCash::checkInitilisation(){

	assert(agents!=nullptr);

}

void DataItemCollectorAmountOfCash::setAgents(std::vector<Agent*>* newAgents){

	agents = newAgents;

}
