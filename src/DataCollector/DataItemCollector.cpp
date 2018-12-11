/*
 * DataItemCollector.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: Cramer
 */


#include "DataItemCollector.h"
#include <limits>
#include <iostream>


/** Constructor of the DataItemCollector
 */
DataItemCollector::DataItemCollector():
     name("")
    , collectInterval(1)
    , collectCount(1)
    , groupToTrack_(-1)
{ }

/** Destructor of the DataItemCollector
 */
DataItemCollector::~DataItemCollector(){
	for(auto& row: dataMatrix){
	    row.clear();
	}
	dataMatrix.clear();
}



/** Is called by the simulation every time step and collects data if the counter is full.
 * Also writes to disk if the counter is full.
 */
void DataItemCollector::collect(){
	if(collectCount == collectInterval){
		collectData();
		collectCount = 1;
	} else {
		collectCount += 1;
	}


}


/** Setter method for the name.
 */
void DataItemCollector::setName(const std::string &newName){
    name = newName;
}

/** Getter method for the name.
 */
std::string DataItemCollector::getName() const{
    return name;
}



/** Set the interval in which data is collected.
 */
void DataItemCollector::setCollectInterval(int newCollectInterval){
	assert(newCollectInterval > 0);
	collectInterval = newCollectInterval;
	collectCount = 1;
}


void DataItemCollector::setGroupToTrack(int groupToTrack){
	assert(groupToTrack > -2);
	groupToTrack_ = groupToTrack;
}

std::string DataItemCollector::methodToString(Method method){
	std::string methodString;
	if(method == DataItemCollector::Method::STD){
		methodString = "std";
	}
	else if(method == DataItemCollector::Method::DETAIL){
		methodString = "detail";
	}
	else if(method == DataItemCollector::Method::EXTREME_PROPORTIONS){
		methodString = "extreme_proportions";
	}
	else if(method == DataItemCollector::Method::MEAN_NOISE_IMPACT){
		methodString = "mean_noise_impact";
	}
	else { // Default method is mean
		methodString = "mean";
	}
	return methodString;

}

DataItemCollector::Method DataItemCollector::stringToDataItemCollectorMethod(const std::string &methodString){
	DataItemCollector::Method method;
	if(methodString == std::string("std")){
		method = DataItemCollector::Method::STD;
	}
	else if(methodString == std::string("detail")){
		method = DataItemCollector::Method::DETAIL;
	}
	else if (methodString == std::string("extreme_proportions")){
		method = DataItemCollector::Method::EXTREME_PROPORTIONS;
	}
	else if (methodString == std::string("mean_noise_impact")){
		method = DataItemCollector::Method::MEAN_NOISE_IMPACT;
	}
	else { // Default method is mean
		method = DataItemCollector::Method::MEAN;
	}

	return method;
}


void DataItemCollector::clearData(){
	dataMatrix.clear();
}

std::vector<std::vector<double>> * DataItemCollector::getData(){
	return &dataMatrix;
}

