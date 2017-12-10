/*
 * DataItemCollector.h
 *
 *  Created on: 01.05.2016
 *      Author: Cramer
 */

#ifndef DataItemCollector_H_
#define DataItemCollector_H_

#include <string>
#include "../Writer/Writer.h"

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif


/** Pure-Virtual Base class for all implementations of a data collector.
 * Contains the logic for collecting data and writing in regular intervals throughout the simulation.
 * Adds the group logic.
 */
class DataItemCollector{
#if BUILD_TESTS
    FRIEND_TEST(DataItemCollector, setWriter);
    FRIEND_TEST(DataItemCollector, setName);
    FRIEND_TEST(DataItemCollector, setWriteInterval);
    FRIEND_TEST(DataItemCollector, setCollectInterval);
    FRIEND_TEST(DataItemCollector, setGroupToTrack);
#endif

protected:
	Writer* writer; /**< Pointer to the Writer */

	std::string name; /**< Name of the DataCollector. Also the name of the file on disk. */
	int writeInterval; /**< Interval in time steps in which the data is written by the writer.
 * Default: 0 --> It is never written automatically*/
	int writeCount; /**< Counter for the time steps. */
	int collectInterval; /**< Interval in time steps in which the data is collected from the simulation.
 * Default: 0 --> Data is collected every timestep. */
	int collectCount; /**< Counter for the time steps. */

	int groupToTrack_; /**< GroupID that this DataCollector is Tracking. By default -1 and tracks all */

	virtual void collectData() = 0;

public:
	DataItemCollector();
	virtual ~DataItemCollector();

	void setWriter(Writer* newWriter);
	void collect();
    void setName(const std::string& newName);
    std::string getName() const;
	void setWriteInterval(int newWriteInterval);
	void setCollectInterval(int newCollectInterval);
	void setGroupToTrack(int groupToTrack);

	virtual void write() = 0;
	virtual void clearData() = 0;
	virtual void checkInitilisation() = 0;

};

#endif /* DataItemCollector_H_ */
