/*
 * DataItemCollectorAmountOfCash.h
 *
 *  Created on: Aug 4, 2016
 *      Author: cramer
 */

#ifndef DATACOLLECTOR_DATAITEMCOLLECTORAMOUNTOFCASH_H_
#define DATACOLLECTOR_DATAITEMCOLLECTORAMOUNTOFCASH_H_

#include <vector>

#include "../Agent/Agent.h"
#include "../Util/Util.h"
#include "DataItemCollector.h"
#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

class DataItemCollectorAmountOfCash: public DataItemCollector {
#if BUILD_TESTS
FRIEND_TEST(DataItemCollectorAmountOfCash, collectDataMean);
FRIEND_TEST(DataItemCollectorAmountOfCash, collectDataStd);
FRIEND_TEST(DataItemCollectorAmountOfCash, clearData);
FRIEND_TEST(DataItemCollectorAmountOfCash, collectData_with1Group);
FRIEND_TEST(DataItemCollectorAmountOfCash, collectData_with2Group);
FRIEND_TEST(DataItemCollectorAmountOfCash, collectData_with2GroupPerAgent);
FRIEND_TEST(DataItemCollectorAmountOfCash, collectDataPerAgent);
FRIEND_TEST(DataItemCollectorAmountOfCash, checkInitilisation);
FRIEND_TEST(DataItemCollectorAmountOfCash, setPerAgents);
FRIEND_TEST(DataItemCollectorAmountOfCash, setAgents);
#endif
private:
	std::vector<Agent*>* agents;
	Method method;
protected:
	 virtual void  collectData() ;
public:
	DataItemCollectorAmountOfCash();

	explicit DataItemCollectorAmountOfCash(std::string method);
	explicit DataItemCollectorAmountOfCash(DataItemCollector::Method method);

	virtual ~DataItemCollectorAmountOfCash();


	 virtual void  checkInitilisation() ;

	virtual std::vector<std::vector<double>> * getData();

	void setAgents(std::vector<Agent*>* newAgents);
};

#endif /* DATACOLLECTOR_DATAITEMCOLLECTORAMOUNTOFCASH_H_ */
