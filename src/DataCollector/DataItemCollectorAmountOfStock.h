/*
 * DataItemCollectorAmountOfStock.h
 *
 *  Created on: Aug 4, 2016
 *      Author: cramer
 */

#ifndef DATACOLLECTOR_DATAITEMCOLLECTORAMOUNTOFSTOCK_H_
#define DATACOLLECTOR_DATAITEMCOLLECTORAMOUNTOFSTOCK_H_

#include <vector>
#include <string>
#include "../Agent/Agent.h"
#include "../Util/Util.h"
#include "DataItemCollector.h"
#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

class DataItemCollectorAmountOfStock: public DataItemCollector {
#if BUILD_TESTS
	FRIEND_TEST(DataItemCollectorAmountOfStock, collectData);
	FRIEND_TEST(DataItemCollectorAmountOfStock, clearData);
	FRIEND_TEST(DataItemCollectorAmountOfStock, collectData_with1Group);
	FRIEND_TEST(DataItemCollectorAmountOfStock, collectData_with2Group);
	FRIEND_TEST(DataItemCollectorAmountOfStock, checkInitilisation);
	FRIEND_TEST(DataItemCollectorAmountOfStock, setAgents);
#endif
private:
	std::vector<Agent*>* agents;
	Method method;

protected:
	 virtual void  collectData() ;

public:
	DataItemCollectorAmountOfStock();

	explicit DataItemCollectorAmountOfStock(std::string method);

	virtual ~DataItemCollectorAmountOfStock();

	virtual std::vector<std::vector<double>> * getData();

	 virtual void  checkInitilisation() ;

	void setAgents(std::vector<Agent*>* newAgents);
};

#endif /* DATACOLLECTOR_DATAITEMCOLLECTORAMOUNTOFSTOCK_H_ */
