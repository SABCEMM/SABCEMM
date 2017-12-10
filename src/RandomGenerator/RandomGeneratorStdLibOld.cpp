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
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include "RandomGeneratorStdLibOld.h"
#include "Algorithms/sabcemm_rand.cc"
#include "../Util/Util.h"





//#define M_PI           3.14159265358979323846  /* pi */

/** Standard constructor for RandomGeneratorStdLibOld. Calls a different constructor with a seed from the randomDevice.
 */
RandomGeneratorStdLibOld::RandomGeneratorStdLibOld():  RandomGeneratorStdLibOld(createSeed()){
}

/** Constructor for RandomGeneratorStdLibOld with user specified seed.
 * \param seed Seed for the randomGenerator.
 */
RandomGeneratorStdLibOld::RandomGeneratorStdLibOld(int seed)
    : RandomGenerator(abs(seed))
{
    SABCEMM_RAND::srand(static_cast<unsigned int>(this->seed));
}


/** Standard destructor for RandomGeneratorStdLibOld.
 */
RandomGeneratorStdLibOld::~RandomGeneratorStdLibOld() = default;

void RandomGeneratorStdLibOld::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) {
	double tempRand = 0;
	getUniformRandomDouble(lowerBound, upperBound, &tempRand);
	*randNumber = int(std::round(tempRand));
}

void RandomGeneratorStdLibOld::getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers,
                                                   std::size_t size) {
	randNumbers->clear();
	for(std::size_t i = 0; i< size; i++){
		int tempRand = 0;
		getUniformRandomInt(lowerBound, upperBound, &tempRand);
		randNumbers->push_back(tempRand);
	}
}

void RandomGeneratorStdLibOld::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {
	if(Util::doubleEqual(lowerBound,upperBound)){
		*randNumber = upperBound;
		return;
	}

	int32_t randNumber32 = 0;
	SABCEMM_RAND::rand(&randNumber32);

	//Bring randNumber from [0,RAND_MAX] to [0,1]
	//double tempRandNumber = randNumber32/RAND_MAX;
	double tempRandNumber = randNumber32/pow(2,31);


	//Transform to requested range
	*randNumber = lowerBound + (upperBound-lowerBound) * tempRandNumber;
}

void RandomGeneratorStdLibOld::getUniformRandomDouble(double lowerBound, double upperBound,
													  std::vector<double> *randNumbers, std::size_t size) {
	randNumbers->clear();
	for(std::size_t i = 0; i< size; i++){
		double tempRand = 0;
		getUniformRandomDouble(lowerBound, upperBound, &tempRand);
		randNumbers->push_back(tempRand);
	}
}


void RandomGeneratorStdLibOld::getNormalRandomInt(double mu, double sigma, int* randNumber) {
	double tempRand = 0;
	getNormalRandomDouble(mu, sigma, &tempRand);
	*randNumber = int(std::round(tempRand));
}

void RandomGeneratorStdLibOld::getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) {
	randNumbers->clear();
	for(std::size_t i = 0; i< size; i++){
		int tempRand = 0;
		getNormalRandomInt(mu, sigma, &tempRand);
		randNumbers->push_back(tempRand);
	}
}

void RandomGeneratorStdLibOld::getNormalRandomDouble(double mu, double sigma, double* randNumber) {
	//Box M�ller Transformation
	double n1 = 0;
	double n2 = 0;

	boxMuellerTransformation(n1,n2);

	*randNumber = n1*sigma + mu;
}

void RandomGeneratorStdLibOld::getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers,
													 std::size_t size) {
	randNumbers->clear();

	while(randNumbers->size()< size){
		double n1 = 0;
		double n2 = 0;
		boxMuellerTransformation(n1,n2);
		randNumbers->push_back(n1*sigma + mu);
		randNumbers->push_back(n2*sigma + mu);
	}
	//Since we always add two numbers, we might have to remove one
	while(randNumbers->size()>size){
		randNumbers->pop_back();
	}
}

void RandomGeneratorStdLibOld::boxMuellerTransformation(double& n1, double& n2){
	//Box M�ller Transformation
	double u1=0;
	double u2=0;

	getUniformRandomDouble(0,1,&u1);
	getUniformRandomDouble(0,1,&u2);

	n1 = cos(2*M_PI*u2)*sqrt(-2*log(u1));
	n2 = sin(2*M_PI*u2)*sqrt(-2*log(u1));
}
