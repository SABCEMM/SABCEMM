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
 * @file LatticeNeighbourhoodGeneratorMatlab.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef LATTICENEIGHBOURHOODGENERATORMATLAB_H_
#define LATTICENEIGHBOURHOODGENERATORMATLAB_H_

#include "NeighbourhoodGenerator.h"
#include "../Agent/Agent.h"


/** Class that generates a neighbourhood of agents living on a square lattice.
 *  Implements NeighbourhoodGenerator.
 *
 *  Note that this generator places the origin at the upper left corner,
 *   which is exactly what the original implementation did.
 *
 *  The generated grids _should_ be the same. Small evidence is that
 *   this class also passes the current LatticeNeighbourhoodGeneratorTest.
 *
 *  You must use this generator to check the program against the matlab
 *   implementation. Otherwise, you should use LatticeNeighbourhoodGenerator.
 *
 * @deprecated
 *
 * @see LatticeNeighbourhoodGenerator
 */
class LatticeNeighbourhoodGeneratorMatlab: public NeighbourhoodGenerator {
public:
    explicit LatticeNeighbourhoodGeneratorMatlab(vector<Agent*>* newAgents);

	virtual ~LatticeNeighbourhoodGeneratorMatlab();
	 virtual void  generateNeighbourhood() ;
};

#endif
