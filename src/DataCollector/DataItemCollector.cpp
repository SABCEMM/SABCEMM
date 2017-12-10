/*
 * DataItemCollector.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: Cramer
 */

#include "DataItemCollector.h"


/** Constructor of the DataItemCollector
 */
DataItemCollector::DataItemCollector():
      writer(nullptr)
    , name("")
    , writeInterval(INT_MAX)
    , writeCount(1)
    , collectInterval(1)
    , collectCount(1)
    , groupToTrack_(-1)
{ }

/** Destructor of the DataItemCollector
 */
DataItemCollector::~DataItemCollector() = default;


/** Setter method for the writer.
 */
void DataItemCollector::setWriter(Writer* newWriter){
	writer = newWriter;
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

	if(writeCount == writeInterval){
		write();
		clearData();
		writeCount = 1;
	} else {
		writeCount += 1;
	}

}


/** Setter method for the name.
 */
void DataItemCollector::setName(const string &newName){
    name = newName;
}

/** Getter method for the name.
 */
std::string DataItemCollector::getName() const{
    return name;
}

/** Set the interval in which is written.
 */
void DataItemCollector::setWriteInterval(int newWriteInterval){
	assert(newWriteInterval > 0);
	writeInterval = newWriteInterval;
	writeCount = 1;
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



