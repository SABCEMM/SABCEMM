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

#ifndef RANDOMGENERATOR_RANDOMNUMBERPOOL_H_
#define RANDOMGENERATOR_RANDOMNUMBERPOOL_H_

#include <vector>
#include <cstddef> //for std::size_t
#include "RandomGenerator.h"
#include "../Writer/Writer.h"

class RandomNumberPool: public RandomGenerator {
private:
	std::size_t uniformPoolInitialSize;
	std::size_t normalPoolInitialSize;

	std::size_t normalPoolFills;
	std::size_t uniformPoolFills;

	std::size_t uniformPoolUnused;
	std::size_t normalPoolUnused;

	RandomGenerator* randomGenerator;
	std::vector<double> uniformPool;
	std::vector<double> normalPool;

    void fillUniformPool();
    void fillNormalPool();


public:
	RandomNumberPool();
	RandomNumberPool(std::size_t newUniformPoolInitialSize, std::size_t newNormalPoolInitialSize);

	virtual ~RandomNumberPool();
	void clearPools();
	void getUsageInformation(std::size_t& uniformGenerated, std::size_t& uniformUnused,
			std::size_t& normalGenerated, std::size_t& normalUnused, int& seed);

	void setRandomGenerator(RandomGenerator* newRandomGenerator);

	void setUniformPoolSize(std::size_t newUniformPoolSize);
	void setNormalPoolSize(std::size_t newNormalPoolSize);

	 virtual void  getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) ;

	 virtual void  getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers, std::size_t size) ;

	 virtual void  getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) ;

	 virtual void  getUniformRandomDouble(double lowerBound, double upperBound, std::vector<double> *randNumbers, std::size_t size) ;

	 virtual void  getNormalRandomInt(double mu, double sigma, int* randNumber) ;

	 virtual void  getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) ;

	 virtual void  getNormalRandomDouble(double mu, double sigma, double* randNumber) ;

	 virtual void  getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers, std::size_t size) ;

};

#endif /* RANDOMGENERATOR_RANDOMNUMBERPOOL_H_ */
