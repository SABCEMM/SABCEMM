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
#include <cstddef> //for std::size_t
#include <random>
#include "RandomGeneratorStdLib.h"





/** Standard constructor for RandomGeneratorStdLib. Calls a different constructor with a seed from the randomDevice.
 */
RandomGeneratorStdLib::RandomGeneratorStdLib():  RandomGeneratorStdLib(createSeed()){
}

/** Constructor for RandomGeneratorStdLib with user specified seed.
 * \param seed Seed for the randomGenerator.
 */
RandomGeneratorStdLib::RandomGeneratorStdLib(int seed) :
    RandomGenerator(seed)
{
	randomGenerator = new std::mt19937_64(static_cast<unsigned>(seed));
	//randomGenerator = new std::mt19937(getSeed());
	//randomGenerator = new std::mt19937;
}


/** Standard destructor for RandomGeneratorStdLib.
 */
RandomGeneratorStdLib::~RandomGeneratorStdLib() {
	delete randomGenerator;
}

void RandomGeneratorStdLib::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) {
	std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
	*randNumber = distribution(*randomGenerator);
}

void RandomGeneratorStdLib::getUniformRandomInt(int lowerBound, int upperBound,
                                                std::vector<int> *randNumbers, std::size_t size) {
	std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
	randNumbers->clear();
	randNumbers->resize(size);
	for(std::size_t i=0;i<size;i++)
	{
		(*randNumbers)[i] = distribution(*randomGenerator);
	}
}

void RandomGeneratorStdLib::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {
	std::uniform_real_distribution<double> distribution(lowerBound, upperBound);
	*randNumber = distribution(*randomGenerator);
}

void RandomGeneratorStdLib::getUniformRandomDouble(double lowerBound, double upperBound,
												   std::vector<double> *randNumbers, std::size_t size) {
	std::uniform_real_distribution<double> distribution(lowerBound, upperBound);
	randNumbers->clear();
	randNumbers->resize(size);
	for(std::size_t i=0;i<size;i++)
	{
		(*randNumbers)[i] = distribution(*randomGenerator);
	}
}


void RandomGeneratorStdLib::getNormalRandomInt(double mu, double sigma, int* randNumber) {
	std::normal_distribution<double> distribution(mu, sigma);
	*randNumber = int(distribution(*randomGenerator));
}

void RandomGeneratorStdLib::getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) {
	std::normal_distribution<double> distribution(mu, sigma);
	randNumbers->clear();
	randNumbers->resize(size);
	for(std::size_t i=0;i<size;i++)
	{
		(*randNumbers)[i] = int(distribution(*randomGenerator));
	}
}

void RandomGeneratorStdLib::getNormalRandomDouble(double mu, double sigma, double* randNumber) {
	std::normal_distribution<double> distribution(mu, sigma);
	*randNumber = distribution(*randomGenerator);
}

void RandomGeneratorStdLib::getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers,
												  std::size_t size) {
	std::normal_distribution<double> distribution(mu, sigma);
	randNumbers->clear();
	randNumbers->resize(size);
	for(std::size_t i=0;i<size;i++)
	{
		(*randNumbers)[i] = distribution(*randomGenerator);
	}
}
