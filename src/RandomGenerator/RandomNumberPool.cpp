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

//#include "omp.h"
#include <cstddef> //for std::size_t
#include "RandomNumberPool.h"
#include<algorithm>


/** Standard Constructor for the RandomNumberPool. Pool sizes for normal and uniform distribution are set to one.
 */
RandomNumberPool::RandomNumberPool() : RandomNumberPool(1,1){


}

/** Constructor for the RandomNumberPool. Pool sizes are set via parameters.
 * \param newUniformPoolInitialSize Size of the uniform pool
 * \param newNormalPoolInitialSize Size of the normal pool
 */
RandomNumberPool::RandomNumberPool(std::size_t newUniformPoolInitialSize, std::size_t newNormalPoolInitialSize) :
    RandomGenerator(-1)
{

	randomGenerator = nullptr;
	uniformPoolInitialSize = newUniformPoolInitialSize;
	normalPoolInitialSize = newNormalPoolInitialSize;
	uniformPool.clear();
	uniformPool.reserve(uniformPoolInitialSize);
	normalPool.clear();
	normalPool.reserve(normalPoolInitialSize);
	uniformPoolFills = 0;
	normalPoolFills = 0;
	uniformPoolUnused = 0;
	normalPoolUnused = 0;
}

/** Destructor of the RandomNumberPool
 */
RandomNumberPool::~RandomNumberPool() {

	clearPools();

	delete randomGenerator;
	randomGenerator = nullptr;
}

/** Set the RandomGenerator to fill the pools.
 * \param newRandomGenerator Pointer to the RandomGenerator
 */
void RandomNumberPool::setRandomGenerator(RandomGenerator* newRandomGenerator) {

	randomGenerator = newRandomGenerator;

}

/** Fill the uniform pool with random numbers between 0 and 1.
 */
void RandomNumberPool::fillUniformPool() {


	std::size_t amountToGenerate = uniformPoolInitialSize - uniformPool.size();

	std::vector<double> tempVector;
	tempVector.clear();
	randomGenerator->getUniformRandomDouble(0, 1, &tempVector, amountToGenerate);
	uniformPool.insert(uniformPool.end(), tempVector.begin(), tempVector.end());
	tempVector.clear();

	uniformPoolFills++;


}

/** Fill the uniform pool with random numbers from a normal distribution N(0,1)
 */
void RandomNumberPool::fillNormalPool() {


	std::size_t amountToGenerate = normalPoolInitialSize - normalPool.size();

	std::vector<double> tempVector;
	tempVector.clear();
	randomGenerator->getNormalRandomDouble(0, 1, &tempVector, amountToGenerate);
	normalPool.insert(normalPool.end(), tempVector.begin(), tempVector.end());
	tempVector.clear();

	normalPoolFills++;


}

/** Returns one random integer from the uniform distribution. The number is taken from the pool and mapped into the
 * right interval.
 * \param lowerBound
 * \param upperBound
 * \param randNumber Pointer to the number
 */
void RandomNumberPool::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) {


	double tempRandNumber;
	getUniformRandomDouble(lowerBound, upperBound, &tempRandNumber);
	*randNumber = int(std::round(tempRandNumber));


}

/** Returns a vector of integers from the uniform distribution. Attention: The vector is cleared before it is written.
 * The number is taken from the pool and mapped into the right interval.
 * \param lowerBound
 * \param upperBound
 * \param randNumbers Pointer to a vector of type double.
 * \param size Amount of random numbers to generate
 */
void RandomNumberPool::getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers, std::size_t size) {


	std::vector<double> tempRandomVector;
	tempRandomVector.clear();
	getUniformRandomDouble(double(lowerBound), double(upperBound), &tempRandomVector, size);
	randNumbers->clear();
	std::for_each(tempRandomVector.begin(), tempRandomVector.end(), [](double& d) { d=std::round(d);});
	randNumbers->insert(randNumbers->end(), tempRandomVector.begin(), tempRandomVector.end());
	tempRandomVector.clear();


}

/** Returns one random double from the uniform distribution.
 * The number is taken from the pool and mapped into the right interval.
 * \param lowerBound
 * \param upperBound
 * \param randNumber Pointer to the number
 */
void RandomNumberPool::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {


	if(uniformPool.empty()){
		fillUniformPool();
	}
	*randNumber = lowerBound + uniformPool.back() * (upperBound - lowerBound);
	uniformPool.pop_back();


}

/** Returns a vector of doubles from the uniform distribution. Attention: The vector is cleared before it is written.
 * The number is taken from the pool and mapped into the right interval.
 * \param lowerBound
 * \param upperBound
 * \param randNumbers Pointer to a vector of type double.
 * \param size Amount of random numbers to generate
 */
void RandomNumberPool::getUniformRandomDouble(double lowerBound, double upperBound,
											  std::vector<double> *randNumbers, std::size_t size) {


	if(uniformPoolInitialSize < size){
		uniformPoolInitialSize = size;
		fillUniformPool();

	}

	if(uniformPool.size() < size){
		fillUniformPool();
	}

	randNumbers->clear();
	auto it_begin = uniformPool.begin();
	auto it_end = uniformPool.begin();
    std::advance(it_end, static_cast<int>(size));
	randNumbers->insert(randNumbers->end(), it_begin, it_end);
	uniformPool.erase(it_begin, it_end);
	std::for_each(randNumbers->begin(), randNumbers->end(), [&lowerBound, &upperBound](double& d)
	{ d=lowerBound+(upperBound-lowerBound)*d;});


}

/** Returns one random integer from the normal distribution.
 * The number is taken from the pool and mapped into the desired distribution.
 * \param mu
 * \param sigma
 * \param randNumber Pointer to the number (double)
 */
void RandomNumberPool::getNormalRandomInt(double mu, double sigma, int* randNumber) {


	double tempRandNumber;
	getNormalRandomDouble(mu, sigma, &tempRandNumber);
	*randNumber = int(std::round(tempRandNumber));


}

/** Returns a vector of integers from the normal distribution. Attention: The vector is cleared before it is written.
 * The number is taken from the pool and mapped into the desired distribution.
 * \param mu
 * \param sigma
 * \param randNumbers Pointer to a vector of type double.
 * \param size Amount of random numbers to generate
 */
void RandomNumberPool::getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) {


	std::vector<double> tempRandomVector;
	tempRandomVector.clear();
	getNormalRandomDouble(mu, sigma, &tempRandomVector, size);
	randNumbers->clear();
	std::for_each(tempRandomVector.begin(), tempRandomVector.end(), [](double& d) { d=std::round(d);});
	randNumbers->insert(randNumbers->end(), tempRandomVector.begin(), tempRandomVector.end());
	tempRandomVector.clear();


}

/** Returns one random double from the normal distribution.
 * The number is taken from the pool and mapped into the desired distribution.
 * \param mu
 * \param sigma
 * \param randNumber
 */
void RandomNumberPool::getNormalRandomDouble(double mu, double sigma, double* randNumber) {


	if(normalPool.empty()){
		fillNormalPool();
	}
	*randNumber = normalPool.back() * sigma + mu;
	normalPool.pop_back();


}

/** Returns a vector of doubles from the normal distribution. Attention: The vector is cleared before it is written.
 * The number is taken from the pool and mapped into the desired distribution.
 * \param mu
 * \param sigma
 * \param randNumbers  Pointer to a vector of type double.
 * \param size Amount of random numbers to generate
 */
void RandomNumberPool::getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers, std::size_t size) {


	if(normalPoolInitialSize < size){
		normalPoolInitialSize = size;
		fillNormalPool();

	}

	if(normalPool.size() < size){
		fillNormalPool();
	}

	auto it_begin = normalPool.begin();
    auto it_end = normalPool.begin();
    std::advance(it_end, static_cast<int>(size));
	randNumbers->clear();
	randNumbers->insert(randNumbers->end(), it_begin, it_end);
	normalPool.erase(it_begin, it_end);
	std::for_each(randNumbers->begin(), randNumbers->end(), [&sigma, &mu](double& d) { d = d * sigma + mu;});


}


void RandomNumberPool::clearPools(){
	uniformPoolUnused += uniformPool.size();
	normalPoolUnused += normalPool.size();
	normalPool.clear();
	normalPool.shrink_to_fit();
	uniformPool.clear();
	uniformPool.shrink_to_fit();
}

void RandomNumberPool::getUsageInformation(std::size_t& uniformGenerated, std::size_t& uniformUnused,
										   std::size_t& normalGenerated, std::size_t& normalUnused, int& seed){
	uniformGenerated = uniformPoolFills * uniformPoolInitialSize;
	uniformUnused = uniformPoolUnused + uniformPool.size();
	normalGenerated = normalPoolFills * normalPoolInitialSize;
	normalUnused = normalPoolUnused + normalPool.size();
	seed = randomGenerator->getSeed();
}
