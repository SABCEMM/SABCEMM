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

#ifndef _RandomGenerator_hpp_
#define _RandomGenerator_hpp_

#include <vector>
#if BUILD_TESTS
//Necessary include to test private functions with googleTest
#include "gtest/gtest_prod.h"
#endif

#include "../Parameter/Parameter.h"
#include "../Writer/Writer.h"

/** Pure-Virutal Class to generate random numbers. Defines a common interface for all random generators.
 * @todo make the functions that take vectors templates, such that any container can be used
 *       (e.g. deques would come in handy)
 */
class RandomGenerator {
#if BUILD_TESTS
	FRIEND_TEST(RandomGeneratorTest, seed);
#endif
protected:
    int seed;
    static int createSeed();
public:
    virtual ~RandomGenerator() = default;

    RandomGenerator(int seed);

    int getSeed(){return seed;}

	static RandomGenerator* factory(Parameter* parameter, Writer* writer);

	/** Returns one random integer from the uniform distribution.
     * \param lowerBound drawn numbers are larger than lowerBound
     * \param upperBound drawn numbers are smaller than upperBOund
     * \param randNumber Pointer to the number
     */
	virtual void getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) = 0;

    /** Returns a vector of integers from the uniform distribution. Attention: The vector is cleared before it is written.
     * \param lowerBound drawn numbers are larger than lowerBound
     * \param upperBound drawn numbers are smaller than upperBOund
     * \param randNumbers Pointer to a vector of type double.
     * \param size Amount of random numbers to generate
     */
	virtual void getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers, std::size_t size) = 0;

    /** Returns one random double from the uniform distribution.
     * \param lowerBound drawn numbers are larger than lowerBound
     * \param upperBound drawn numbers are smaller than upperBOund
     * \param randNumber Pointer to the number
     */
	virtual void getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) = 0;

    /** Returns a vector of doubles from the uniform distribution. Attention: The vector is cleared before it is written.
     * \param lowerBound drawn numbers are larger than lowerBound
     * \param upperBound drawn numbers are smaller than upperBOund
     * \param randNumbers Pointer to a vector of type double.
     * \param size Amount of random numbers to generate
     */
	virtual void getUniformRandomDouble(double lowerBound, double upperBound, std::vector<double> *randNumbers,
										std::size_t size) = 0;

    /** Returns one random integer from the normal distribution.
     * \param mu mean
     * \param sigma standard deviation
     * \param randNumber Pointer to the number
     */
	virtual void getNormalRandomInt(double mu, double sigma, int* randNumber) = 0;

    /** Returns a vector of integers from the normal distribution. Attention: The vector is cleared before it is written.
     * \param mu mean
     * \param sigma standard deviation
     * \param randNumbers Pointer to a vector of type double.
     * \param size Amount of random numbers to generate
     */
	virtual void getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) = 0;

    /** Returns one random double from the normal distribution.
     * \param mu mean
     * \param sigma standard deviation
     * \param randNumber Pointer to the number
     */
	virtual void getNormalRandomDouble(double mu, double sigma, double* randNumber) = 0;

    /** Returns a vector of doubles from the normal distribution. Attention: The vector is cleared before it is written.
     * \param mu mean
     * \param sigma standard deviation
     * \param randNumbers  Pointer to a vector of type double.
     * \param size Amount of random numbers to generate
     */
	virtual void getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers, std::size_t size) = 0;
};


#endif
