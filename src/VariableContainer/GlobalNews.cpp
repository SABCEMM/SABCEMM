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

#include <cassert>

#include "GlobalNews.h"


/** Standardconstructor
 */
GlobalNews::GlobalNews(): GlobalNews(nullptr) {


}


/** Constructor
 * \param newRandomGenerator Pointer to a RandomGenerator
 */
GlobalNews::GlobalNews(RandomGenerator* newRandomGenerator):
randomGenerator(newRandomGenerator)
{

	assert(randomGenerator != nullptr);
	randomGenerator->getNormalRandomDouble(0,1,&curGlobalNews);
	randomGenerator->getNormalRandomDouble(0,1,&prevGlobalNews);


}


/** Destructor
 */
GlobalNews::~GlobalNews() = default;


/** Getter method for the current global information
 * \return Current global information
 */
const double& GlobalNews::getCurGlobalNews() const{


	return curGlobalNews;
}


/** Getter method for the previous global information
 * \return Previous global information
 */
const double& GlobalNews::getPrevGlobalNews() const{


	return prevGlobalNews;
}


/** Generate new global information.
 */
void GlobalNews::generateNewGlobalNews(){


	assert(randomGenerator != nullptr);
	prevGlobalNews = curGlobalNews;
	randomGenerator->getNormalRandomDouble(0,1,&curGlobalNews);


}
