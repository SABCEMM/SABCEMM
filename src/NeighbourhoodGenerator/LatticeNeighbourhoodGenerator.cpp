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
 * @file LatticeNeighbourhoodGenerator.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "LatticeNeighbourhoodGenerator.h"
#include "../Agent/AgentHarras.h"
#include <cstddef> //for std::size_t
#include <cassert>
#include <cmath>

/** Constructor of the LatticeNeighbourhoodGenerator. Requires a pointer to an vector of AgentHarras.
 */
LatticeNeighbourhoodGenerator::LatticeNeighbourhoodGenerator(std::vector<Agent*>* newAgents):
NeighbourhoodGenerator(newAgents){
}


/** Standarddestructor
 */
LatticeNeighbourhoodGenerator::~LatticeNeighbourhoodGenerator() = default;


/**
 * Arranges all agents in a lattice neighbourhood.
 * Agents are assigned based on their position in the vector of all agents.
 * Each agent is assigned the agent above, below, left and right (geometrically) as neighbourhood.
 * Agents on the edges are assigned agents from the corresponding edge on opposite site
 *  (i.e. there is a wrap around).
 */
void LatticeNeighbourhoodGenerator::generateNeighbourhood() {


	assert(countAgents == agents->size());

	//Test if the number of agents is quadratic
	std::size_t l; //size of the neighbourhood is lxl
	l = (std::size_t) sqrt(countAgents);
	assert(l * l == countAgents);

	//typecast the vector of Agents to a vector of HarrasAgents
	//TODO Chekc if this even works and how it behaves on non Harras Agents
    auto * harrasAgents = (std::vector<AgentHarras*>*)agents;


	//Iterate over all Agents and set Pointer to Neighbours
	for (std::size_t i = 0; i < countAgents; i++) {

		harrasAgents->at(i)->clearNeighbours();
		if (i == 0) //Lower left corner
		{
			harrasAgents->at(i)->addNeighbour(agents->at(l-1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+l));
			harrasAgents->at(i)->addNeighbour(agents->at(l * (l - 1)));
		} else if (i == l - 1) //Lower right corner
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i-1));
			harrasAgents->at(i)->addNeighbour(agents->at(0));
			harrasAgents->at(i)->addNeighbour(agents->at(i+l));
			harrasAgents->at(i)->addNeighbour(agents->at((l * l - 1)));
		} else if (i == l * (l - 1)) //Upper left corner
		{
			harrasAgents->at(i)->addNeighbour(agents->at(l * l - 1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+1));
			harrasAgents->at(i)->addNeighbour(agents->at(0));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l));
		} else if (i == countAgents - 1) //Upper right corner
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i-1));
			harrasAgents->at(i)->addNeighbour(agents->at((l * (l - 1))));
			harrasAgents->at(i)->addNeighbour(agents->at(l - 1));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l));
		} else if (i > 0 && i < l - 1) //Upper border
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i-1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+l));
			harrasAgents->at(i)->addNeighbour(agents->at(l*(l-1)+i));
		} else if (i > l * (l - 1) && i < l * l - 1) //Lower border
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i-1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+1));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l*(l-1)));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l));
		} else if (i % l == 0) //Left border
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i+l-1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+l));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l));
		} else if (i % l == l - 1) //Right border
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i-1));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l+1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+l));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l));
		} else //Inside the neighbourhood
		{
			harrasAgents->at(i)->addNeighbour(agents->at(i-1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+1));
			harrasAgents->at(i)->addNeighbour(agents->at(i+l));
			harrasAgents->at(i)->addNeighbour(agents->at(i-l));
		}

	} //End for

}


