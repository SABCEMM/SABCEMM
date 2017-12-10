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

#include "RandomGeneratorFunctional.h"

#include <cmath>
#include <boost/core/ignore_unused.hpp>
#include <cstddef> //for std::size_t
using namespace std;

RandomGeneratorFunctional::RandomGeneratorFunctional(const function<double(std::size_t)> &returnFunction)
    : RandomGenerator(-1), returnFunction(returnFunction), count(0)
{ }

void RandomGeneratorFunctional::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber){
    boost::ignore_unused(upperBound, lowerBound);
    ++count;
    *randNumber = int(round(returnFunction(count)));
}


/** Returns a vector of random integers from the uniform distribution.
 *\param lowerBound
 *\param upperBound
 *\param randNumbers
 *\param size
 *
 */
void RandomGeneratorFunctional::getUniformRandomInt(int lowerBound, int upperBound, std::vector<int>* randNumbers, std::size_t size){
    boost::ignore_unused(upperBound, lowerBound);
    ++count;
    randNumbers->clear();
    randNumbers->resize(size);
    for (std::size_t i=0; i<size; i++)
    {
        randNumbers->at(i) = int(round(returnFunction(count)));
    }
}

/** Returns one random double from the uniform distribution.
 *\param lowerBound
 *\param upperBound
 *\param randNumber
 *
 */
void RandomGeneratorFunctional::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {
    boost::ignore_unused(upperBound, lowerBound);
    ++count;
    *randNumber = returnFunction(count);
}

/** Returns a vector of random doubles from the uniform distribution.
 *\param lowerBound
 *\param upperBound
 *\param randNumbers
 *\param size
 *
 */
void RandomGeneratorFunctional::getUniformRandomDouble(double lowerBound, double upperBound, std::vector<double>* randNumbers, std::size_t size) {
    boost::ignore_unused(upperBound, lowerBound);
    ++count;
    randNumbers->clear();
    randNumbers->resize(size);
    for (std::size_t i=0; i<size; i++)
    {
        randNumbers->at(i) = returnFunction(count);
    }
}


/** Returns one random integer from the normal distribution.
 *\param mu
 *\param sigma
 *\param randNumber
 */
void RandomGeneratorFunctional::getNormalRandomInt(double mu, double sigma, int* randNumber) {
    boost::ignore_unused(mu, sigma);
    ++count;
    *randNumber = int(round(returnFunction(count)));
}


/** Returns a vector of random integers from the normal distribution.
 *\param mu
 *\param sigma
 *\param randNumbers
 *\param size
 */
void RandomGeneratorFunctional::getNormalRandomInt(double mu, double sigma, std::vector<int>* randNumbers, std::size_t size) {
    boost::ignore_unused(mu, sigma);
    ++count;
    randNumbers->clear();
    randNumbers->resize(size);

    for (std::size_t i=0; i<size; i++)
    {
        randNumbers->at(i) = int(round(returnFunction(count)));
    }
}

/** Returns one random double from the normal distribution.
 *\param mu
 *\param sigma
 *\param randNumber
 */
void RandomGeneratorFunctional::getNormalRandomDouble(double mu, double sigma, double* randNumber) {
    boost::ignore_unused(mu, sigma);
    ++count;
    *randNumber = returnFunction(count);
}

/** Returns a vector of random double from the normal distribution.
 *\param mu
 *\param sigma
 *\param randNumbers
 *\param size
 */
void RandomGeneratorFunctional::getNormalRandomDouble(double mu, double sigma, std::vector<double>* randNumbers, std::size_t size) {
    boost::ignore_unused(mu, sigma);
    ++count;
    randNumbers->clear();
    randNumbers->resize(size);

    for (std::size_t i=0; i<size; i++)
    {
        randNumbers->at(i) = returnFunction(count);
    }
}
