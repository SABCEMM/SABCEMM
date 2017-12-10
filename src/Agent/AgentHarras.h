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
 * @file AgentHarras.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef _AgentHarras_hpp_
#define _AgentHarras_hpp_

#include <vector>
#include <cstddef> //for std::size_t
#include "Agent.h"
#include "../RandomGenerator/RandomGenerator.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/GlobalNews.h"
#include "../VariableContainer/ExcessDemand.h"

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

/** Agent from the paper "How to grow a bubble: A model of myopic adapting agents" by Georges Harras and Didier Sornette.
 */
class AgentHarras : public Agent {
#if BUILD_TESTS
FRIEND_TEST(AgentHarrasTest, updateOpinion);
FRIEND_TEST(AgentHarrasTest, updateNeighbourhoodInformation);
FRIEND_TEST(AgentHarrasTest, updateCash);
FRIEND_TEST(AgentHarrasTest, updateStock);
FRIEND_TEST(AgentHarrasTest, updateU);
FRIEND_TEST(AgentHarrasTest, constructor);
FRIEND_TEST(AgentHarrasTest, preStepUpdate);
FRIEND_TEST(AgentHarrasTest, stepUpdate);
FRIEND_TEST(AgentHarrasTest, postStepUpdate);
FRIEND_TEST(DataItemCollectorHarrasK, collectData);
FRIEND_TEST(DataItemCollectorHarrasK, collectDataPerAgent);
FRIEND_TEST(DataItemCollectorHarrasK, collectData_with1Group);
FRIEND_TEST(DataItemCollectorHarrasK, collectData_with2Group);
FRIEND_TEST(DataItemCollectorHarrasK, collectData_with2GroupPerAgent);
FRIEND_TEST(fullSimulationTest, fullSimulation_Harras);
FRIEND_TEST(fullSimulationTest, fullSimulation_Cross);
#endif
protected:
	double c1; /**< Current trust coefficient (neighbourhood information) */
	double c2; /**< Current trust coefficient (global news) */
	double c3; /**< Current trust coefficient (personal news) */
	double threshold; /**< Fixed personal threshold,  */
	double opinion; /**< Current opinion of the agent */
	double alpha; /**< Fixed personal TODO */
	double sigma; /**< Fixed personal TODO */
	double kr;/**< Fixed personal TODO */
	double g; /**< Fixed personal percentage of stock/cash */
	double u; /**< Trust in the GlobalNews */
    double oldU; /**< u of the preceding time step */
	std::vector<Agent*>::size_type countNeighbours; /**< Amount of connected neighbours */
	std::vector<Agent*> neighbours; /**< Pointers to all connected neighbours */
	std::vector<double> k; /**< Current trust in the neighbours */
	double personalInformation; /**< Current personal information */
	double neighbourhoodInformation; /**< Current neighbourhood information */
	std::vector<int> decisionNeighbours; /**< Decisions of the neighbours */

	double prevExcessDemand; /**< Storage for the previous ExcessDemand for later calculations */

	ExcessDemand* excessDemand; /**< Pointer to the ExcessDemand container */
	GlobalNews* globalNews; /**< Pointer to the GlobalNews container */

    double marketDepth; /// @todo refactor - knowledge of marketDepth is a hack
                        ///       and has no conceptual reason.

	void updateCash();
	void updateStock();
	void updateTradingVolume(const double& newPrice);

public:
	AgentHarras();
	AgentHarras(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand, GlobalNews* newGlobalNews);
	AgentHarras(RandomGenerator* newRandomGenerator, Price* newPrice, ExcessDemand* newExcessDemand, GlobalNews* newGlobalNews,
			double newCash, double newStock);
	AgentHarras(RandomGenerator* newRandomGenerator, Price* newPrice,
				ExcessDemand* newExcessDemand, GlobalNews* newGlobalNews,
				double newCash, double newStock,
				double C1, double C2, double C3,
				double Threshold, double newG,
				double newAlpha, double marketDepth);

	void updateOpinion(); 

	void updateNeighbourhoodInformation();

    const std::vector<double>& getK() const;
    void setNeighbours(const std::vector<Agent*>& newNeighbours);
    std::size_t getCountNeighbours() const;
	void updateU();
	void addNeighbour(Agent* newNeighbour);
	void clearNeighbours();

     virtual void  preStepUpdate() ;

     virtual void  stepUpdate() ;

     virtual void  postStepUpdate() ;

     virtual void  updateBisection(const double& newIterPrice) ;
};

#endif
