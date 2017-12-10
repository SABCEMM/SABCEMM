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
 * @file Agent.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef _Agent_hpp_
#define _Agent_hpp_

#include "../RandomGenerator/RandomGenerator.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/DeltaT.h"
#include "../Parameter/Parameter.h"
#include "../Group/Group.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/GlobalNews.h"
#include "../VariableContainer/Dividend.h"

#include <vector>


/** Virtual parent class for all Agent.
 *
 */
class Agent: public Group {

protected:
	double cash; /**< Current cash (Bargeld) of the agent */
	double stock; /**< Current amount of stock of the agent */
	int decision; /**< Last decision of the agent. Depending on implementation of subclass! Often: +1 -> Buying, -1 -> Selling, 0 -> Passive */
	double tradingVolume; /**< Amount of stock to sell/buy at the current timestep. Selling/Buying depends on decision.*/

	RandomGenerator* randomGenerator; /**< Pointer to the random number generator */
	Price* price; /**< Pointer to the Price container */
	DeltaT* deltaT; /**< Pointer to the DeltaT container */

public:
	Agent();
	Agent(RandomGenerator* newRandomGenerator, Price* newPrice);
	Agent(RandomGenerator* newRandomGenerator, Price* newPrice, double newCash, double newStock);
	static std::vector<Agent*>* factory(Parameter* parameter, RandomGenerator* randomNumberPool, Price* price,
										ExcessDemand* excessDemand, GlobalNews* globalNews, Dividend* dividend,
										DeltaT* deltaT);
	virtual ~Agent();

    double getCash() const;
	void setCash(double& newCash);
    const int& getDecision() const;
	void setRandomGenerator(RandomGenerator* newRandomGenerator);
	void setPrice(Price* newPrice);
    double getStock() const;
	void setStock(double& newStock);
    const double& getTradingVolume() const;
	void setDeltaT(DeltaT* newDeltaT);

	virtual void preStepUpdate() = 0;
	virtual void stepUpdate() = 0;
	virtual void postStepUpdate() = 0;

	virtual void updateBisection(const double& newIterPrice) = 0;

};



#endif
