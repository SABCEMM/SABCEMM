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
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "gtest/gtest.h"

#include "../../src/NeighbourhoodGenerator/LatticeNeighbourhoodGenerator.h"
#include "../DummyClasses/RandomGeneratorFunctional.h"
#include "../DummyClasses/AgentDummy.h"
#include <cstddef> //for std::size_t
TEST(LatticeNeighbourhoodGenerator, generateNeighbourhoodDeathTest){
	std::vector<Agent*> newAgents;

	RandomGenerator* randomGenerator;
	randomGenerator = new RandomGeneratorFunctional(ConstantReturn(1));

	for(int i = 0;i < 15; i++){
		AgentDummy* tempAgent = new AgentDummy(randomGenerator, nullptr, 0, 0);
		tempAgent->setTo(1);
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	LatticeNeighbourhoodGenerator neighbourhoodGenerator(&newAgents);

	ASSERT_DEATH(neighbourhoodGenerator.generateNeighbourhood(),"");
	
	for(std::size_t i=0;i<newAgents.size(); i++){
		delete newAgents.at(i);
	}

	delete randomGenerator;
}

TEST(LatticeNeighbourhoodGenerator, generateNeighbourhood){
	std::vector<Agent*> newAgents;
    RandomGeneratorFunctional randomGenerator(ConstantReturn(1));

	for(int i = 0;i < 16; i++){
		AgentDummy* tempAgent = new AgentDummy(&randomGenerator, nullptr);
		tempAgent->setTo(1);
		newAgents.push_back(static_cast<Agent*>(tempAgent));
	}

	LatticeNeighbourhoodGenerator neighbourhoodGenerator(&newAgents);
	
	neighbourhoodGenerator.generateNeighbourhood();

	for(std::size_t i = 0;i < 16; i++){
		EXPECT_EQ(static_cast<AgentDummy*>(newAgents.at(i))->getCountNeighbours(),4);
		EXPECT_EQ(static_cast<AgentDummy*>(newAgents.at(i))->neighbours.size(),4);
	}

	//Every Agent checked by hand to avoid copying original algorithm. std::find returns iterator, which can then be compared to suspected value after demasking.
	//Agent Grid:
/**
	-12--13--14--15-
	 |   |   |   |
	-08--09--10--11-
	 |   |   |   |
	-04--05--06--07-
	 |   |   |   |
	-00--01--02--03-
**/
	
	//Agent 0
	AgentDummy* tempAgent = static_cast<AgentDummy*>(newAgents.at(0));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(3))  ,  newAgents.at(3)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(4))  ,  newAgents.at(4)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(1))  ,  newAgents.at(1)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(12))  ,  newAgents.at(12)  );

	//Agent 1
	tempAgent = static_cast<AgentDummy*>(newAgents.at(1));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(0))  ,  newAgents.at(0)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(5))  ,  newAgents.at(5)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(2))  ,  newAgents.at(2)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(13))  ,  newAgents.at(13)  );

	//Agent 2
	tempAgent = static_cast<AgentDummy*>(newAgents.at(2));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(1))  ,  newAgents.at(1)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(6))  ,  newAgents.at(6)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(3))  ,  newAgents.at(3)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(14))  ,  newAgents.at(14)  );

	//Agent 3
	tempAgent = static_cast<AgentDummy*>(newAgents.at(3));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(2))  ,  newAgents.at(2)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(7))  ,  newAgents.at(7)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(0))  ,  newAgents.at(0)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(15))  ,  newAgents.at(15)  );

	//Agent 4
	tempAgent = static_cast<AgentDummy*>(newAgents.at(4));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(7))  ,  newAgents.at(7)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(8))  ,  newAgents.at(8)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(5))  ,  newAgents.at(5)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(0))  ,  newAgents.at(0)  );

	//Agent 5
	tempAgent = static_cast<AgentDummy*>(newAgents.at(5));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(4))  ,  newAgents.at(4)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(9))  ,  newAgents.at(9)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(6))  ,  newAgents.at(6)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(1))  ,  newAgents.at(1)  );

	//Agent 6
	tempAgent = static_cast<AgentDummy*>(newAgents.at(6));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(5))  ,  newAgents.at(5)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(10))  ,  newAgents.at(10)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(7))  ,  newAgents.at(7)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(2))  ,  newAgents.at(2)  );

	//Agent 7
	tempAgent = static_cast<AgentDummy*>(newAgents.at(7));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(6))  ,  newAgents.at(6)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(11))  ,  newAgents.at(11)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(4))  ,  newAgents.at(4)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(3))  ,  newAgents.at(3)  );

	//Agent 8
	tempAgent = static_cast<AgentDummy*>(newAgents.at(8));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(11))  ,  newAgents.at(11)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(12))  ,  newAgents.at(12)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(9))  ,  newAgents.at(9)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(4))  ,  newAgents.at(4)  );

	//Agent 9
	tempAgent = static_cast<AgentDummy*>(newAgents.at(9));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(8))  ,  newAgents.at(8)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(13))  ,  newAgents.at(13)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(10))  ,  newAgents.at(10)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(5))  ,  newAgents.at(5)  );

	//Agent 10
	tempAgent = static_cast<AgentDummy*>(newAgents.at(10));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(9))  ,  newAgents.at(9)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(14))  ,  newAgents.at(14)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(11))  ,  newAgents.at(11)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(6))  ,  newAgents.at(6)  );

	//Agent 11
	tempAgent = static_cast<AgentDummy*>(newAgents.at(11));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(10))  ,  newAgents.at(10)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(15))  ,  newAgents.at(15)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(8))  ,  newAgents.at(8)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(7))  ,  newAgents.at(7)  );

	//Agent 12
	tempAgent = static_cast<AgentDummy*>(newAgents.at(12));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(15))  ,  newAgents.at(15)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(0))  ,  newAgents.at(0)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(13))  ,  newAgents.at(13)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(8))  ,  newAgents.at(8)  );

	//Agent 13
	tempAgent = static_cast<AgentDummy*>(newAgents.at(13));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(12))  ,  newAgents.at(12)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(1))  ,  newAgents.at(1)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(14))  ,  newAgents.at(14)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(9))  ,  newAgents.at(9)  );

	//Agent 14
	tempAgent = static_cast<AgentDummy*>(newAgents.at(14));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(13))  ,  newAgents.at(13)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(2))  ,  newAgents.at(2)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(15))  ,  newAgents.at(15)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(10))  ,  newAgents.at(10)  );

	//Agent 15
	tempAgent = static_cast<AgentDummy*>(newAgents.at(15));
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(14))  ,  newAgents.at(14)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(3))  ,  newAgents.at(3)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(12))  ,  newAgents.at(12)  );
	EXPECT_EQ(  *std::find(tempAgent->neighbours.begin(), tempAgent->neighbours.end(), newAgents.at(11))  ,  newAgents.at(11)  );

	
	for(std::size_t i=0;i<newAgents.size(); i++){
		delete newAgents.at(i);
	}
}
