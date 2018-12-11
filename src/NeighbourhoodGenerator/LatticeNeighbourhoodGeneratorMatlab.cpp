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
 * @file LatticeNeighbourhoodGeneratorMatlab.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "LatticeNeighbourhoodGeneratorMatlab.h"
#include "../Agent/AgentHarras.h"
#include <cstddef> //for std::size_t
#include <cassert>
#include <cmath>
/** Constructor of the LatticeNeighbourhoodGeneratorMatlab. Requires a pointer to an vector of AgentHarras.
 */
LatticeNeighbourhoodGeneratorMatlab::LatticeNeighbourhoodGeneratorMatlab(std::vector<Agent*>* newAgents):
    NeighbourhoodGenerator(newAgents){
}


/** Standarddestructor
 */
LatticeNeighbourhoodGeneratorMatlab::~LatticeNeighbourhoodGeneratorMatlab() = default;


/** Generates a neighbourhood
 */
void LatticeNeighbourhoodGeneratorMatlab::generateNeighbourhood() {


    assert(countAgents == agents->size());

    //Test if the number of agents is quadratic
    std::size_t l; //size of the neighbourhood is lxl
    l = (std::size_t) sqrt(countAgents);
    assert(l * l == countAgents);

    //typecast the vector of Agents to a vector of HarrasAgents
    //TODO Chekc if this even works and how it behaves on non Harras Agents
    auto * harrasAgents = (std::vector<AgentHarras*>*)agents;


    // l = imax
    // Ursprung liegt in der Ecke unten links.
    //Iterate over all Agents and set Pointer to Neighbours
    for (std::size_t i = 0; i < countAgents; i++) {

        harrasAgents->at(i)->clearNeighbours();
        if(i > 0 && i < l - 1) // upper border
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l)));
            harrasAgents->at(i)->addNeighbour(agents->at((l * (l - 1) + i)));
        }
        else if (i == 0) // upper left corner
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((l - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l * (l - 1))));
        }
        else if (i == l - 1) // upper right corner
        {
            harrasAgents->at(i)->addNeighbour(agents->at(0));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l * (l - 1))));
        }
        else if (i == l * (l - 1)) // Lower left corner
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1 + l)));
            harrasAgents->at(i)->addNeighbour(agents->at(0));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l)));
        }
        else if (i == countAgents - 1) // lower right corner
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i - (l - 1))));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1)));
            // different from matlab (0- vs. 1-indexing)
            harrasAgents->at(i)->addNeighbour(agents->at((l - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l)));
        } else if (i > l * (l - 1) && i < l * l - 1) //Lower border
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l * (l - 1))));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l)));
        }
        else if (i % l == 0) //Left border
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l)));
        }
        else if (i % l == l - 1) //Right border
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i - l + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l)));
        }
        else //Inside the neighbourhood
        {
            harrasAgents->at(i)->addNeighbour(agents->at((i + 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - 1)));
            harrasAgents->at(i)->addNeighbour(agents->at((i + l)));
            harrasAgents->at(i)->addNeighbour(agents->at((i - l)));
        }

    } //End for

}


