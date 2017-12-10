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

#ifndef RANDOMGENERATORINTELMKL_H_
#define RANDOMGENERATORINTELMKL_H_
#include <cstddef> //for std::size_t
#include "mkl.h"
#include "mkl_vsl.h"
#include "RandomGenerator.h"

/**
 * Class generates random number with help of Intel MKL VS.
 *
 * To use this class, you must have WITH_INTEL_MKL enabled in CMake.
 */
class RandomGeneratorIntelMKL: public RandomGenerator{
public:
	RandomGeneratorIntelMKL();
        explicit RandomGeneratorIntelMKL(int seed);
	virtual ~RandomGeneratorIntelMKL();

	 virtual void  getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) ;
	 virtual void  getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers, std::size_t size) ;
	 virtual void  getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) ;
	 virtual void  getUniformRandomDouble(double lowerBound, double upperBound, std::vector<double> *randNumbers, std::size_t size) ;

	 virtual void  getNormalRandomInt(double mu, double sigma, int* randNumber) ;
	 virtual void  getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) ;
	 virtual void  getNormalRandomDouble(double mu, double sigma, double* randNumber) ;
	 virtual void  getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers, std::size_t size) ;

private:
	VSLStreamStatePtr stream;


};

#endif /* RANDOMGENERATORINTELMKL_H_ */
