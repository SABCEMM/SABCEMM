/*
 * DataItemCollector.h
 *
 *  Created on: 01.05.2016
 *      Author: Cramer
 */

#ifndef DataItemCollector_H_
#define DataItemCollector_H_

#include <string>
#include <vector>
#include <cassert>

#if BUILD_TESTS
#include "gtest/gtest_prod.h"

#endif


/** Pure-Virtual Base class for all implementations of a data collector.
 * Contains the logic for collecting data and writing in regular intervals throughout the simulation.
 * Adds the group logic.
 */
class DataItemCollector{
#if BUILD_TESTS
    FRIEND_TEST(DataItemCollector, setName);
    FRIEND_TEST(DataItemCollector, setCollectInterval);
    FRIEND_TEST(DataItemCollector, setGroupToTrack);
	FRIEND_TEST(DataItemCollector, clearData);
#endif


public:

	enum Method
	{
        MEAN,
        MEAN_NOISE_IMPACT,
        STD,
        DETAIL,
        EXTREME_PROPORTIONS

	};
	DataItemCollector();
	virtual ~DataItemCollector();

	void collect();
    void setName(const std::string& newName);
    std::string getName() const;
	void setCollectInterval(int newCollectInterval);
	void setGroupToTrack(int groupToTrack);

	void clearData();
	virtual void checkInitilisation() = 0;
	std::vector<std::vector<double>> * getData();

	static std::string methodToString(Method method);
	static Method stringToDataItemCollectorMethod(const std::string &methodString);



protected:

    std::string name; /**< Name of the DataCollector. Also the name of the file on disk. */
    int collectInterval; /**< Interval in time steps in which the data is collected from the simulation.
 * Default: 0 --> Data is collected every timestep. */
    int collectCount; /**< Counter for the time steps. */

    int groupToTrack_; /**< GroupID that this DataCollector is Tracking. By default -1 and tracks all */

    virtual void collectData() = 0;

    std::vector<std::vector<double>> dataMatrix;

    Method method = Method::MEAN;

};

#endif /* DataItemCollector_H_ */
