/* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Beikirch, Maximilian
 * Cramer, Simon
 * Frank, Martin
 * Otte, Philipp
 * Pabich, Emma
 * Trimborn, Torsten
 *
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file DataItemCollectorHarrasK.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef DATACOLLECTOR_DATAITEMCOLLECTORHARRASK_H_
#define DATACOLLECTOR_DATAITEMCOLLECTORHARRASK_H_

#include <vector>

#include "../Agent/Agent.h"
#include "../Agent/AgentHarras.h"
#include "../Util/Util.h"
#include "DataItemCollector.h"


/** DataCollector for the average trust in the neihgbours (K). Implements DataCollectorBase.
 */
class DataItemCollectorHarrasK: public DataItemCollector {
#if BUILD_TESTS
	FRIEND_TEST(DataItemCollectorHarrasK, collectData);
	FRIEND_TEST(DataItemCollectorHarrasK, clearData);
	FRIEND_TEST(DataItemCollectorHarrasK, collectData_with1Group);
	FRIEND_TEST(DataItemCollectorHarrasK, collectData_with2Group);
	FRIEND_TEST(DataItemCollectorHarrasK, checkInitilisation);
	FRIEND_TEST(DataItemCollectorHarrasK, setAgents);
#endif
private:
	std::vector<AgentHarras*>* agents; /**< Pointer to the agents vector */
	Method method;

protected:
	 virtual void  collectData() ;

public:
	DataItemCollectorHarrasK();

	explicit DataItemCollectorHarrasK(std::vector<Agent*>* newAgents);
	DataItemCollectorHarrasK(std::string method, std::vector<Agent*>* newAgents);

	virtual ~DataItemCollectorHarrasK();

	 virtual void  checkInitilisation() ;

	void setAgents(std::vector<Agent*>* newAgents);
};

#endif /* DATACOLLECTOR_DATAITEMCOLLECTORHARRASK_H_ */
