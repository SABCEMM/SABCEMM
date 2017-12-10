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
#include <cassert>
#include "RandomGeneratorRANDU.h"
#include <cmath>
#include <limits>


//#define M_PI           3.14159265358979323846  /* pi */

/** Standard constructor for RandomGeneratorRANDU. Calls the other constructor and passes a seed from the randomDevice.
 */
RandomGeneratorRANDU::RandomGeneratorRANDU():RandomGeneratorRANDU(createSeed()) {
}

/** Constructor for RandomGeneratorRANDU.
 * \param seed Seed for the randomGEneratorRANDU. Is adjusted such that it is smaller then M and uneven.
 */
RandomGeneratorRANDU::RandomGeneratorRANDU(int seed) :
    RandomGenerator(seed)
{

	if(pow(2,31) > std::numeric_limits<int>::max()){
		M = std::numeric_limits<int>::max();
	}
	else{
		M = (int)pow(2,31);
	}
    assert(M>0);
    A = 0;
    B = 65539;
	this->seed = seed;
	reset();

}


/** Standard destructor for RandomGeneratorRANDU.
 */
RandomGeneratorRANDU::~RandomGeneratorRANDU() = default;

void RandomGeneratorRANDU::reset(){

	oldRandom = abs(seed);
	//The seed/oldRandom has to be smaller then M and positive!
	while(oldRandom > M){

		oldRandom = static_cast<int>(std::round(static_cast<double>(oldRandom)*0.5));


	}
	//The seed has to be odd!
	if(oldRandom % 2 == 0){
		oldRandom--;
	}

}

void RandomGeneratorRANDU::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) {

    double tempRandNumber = 0;

    getUniformRandomDouble(lowerBound, upperBound, &tempRandNumber);

    *randNumber = int(std::round(tempRandNumber)); //scale random number to be between lower bound and upper bound

}

void RandomGeneratorRANDU::getUniformRandomInt(int lowerBound, int upperBound,
                                               std::vector<int> *randNumbers, std::size_t size) {

	int tempRandomNumber = 0;
	randNumbers->clear();
	randNumbers->resize(size);
    for (std::size_t i=0;i<size;i++){
    	getUniformRandomInt(lowerBound, upperBound, &tempRandomNumber);
    	randNumbers->at(i) = tempRandomNumber;
    }

}

void RandomGeneratorRANDU::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {

    double tempRandNumber = 0;
    int64_t newRandom = 0;

    newRandom = A + (B * oldRandom) % M;

    if(newRandom <= 0 || newRandom > M || newRandom == oldRandom){

		reset();
    	getUniformRandomDouble(lowerBound, upperBound, randNumber);
    	return ;
    }

	tempRandNumber = double(oldRandom)/double(M); //generate random number between 0 and 1 and scale to interval
	tempRandNumber = lowerBound + (upperBound-lowerBound)*tempRandNumber; //scale random number to be between
	// lower bound and upper bound
	*randNumber = tempRandNumber;
	oldRandom = newRandom;

}

void RandomGeneratorRANDU::getUniformRandomDouble(double lowerBound, double upperBound,
                                                  std::vector<double> *randNumbers, std::size_t size) {

	double tempRandomNumber = 0;

	randNumbers->clear();
	randNumbers->resize(size);
    for (std::size_t i=0;i<size;i++){
    	getUniformRandomDouble(lowerBound, upperBound, &tempRandomNumber);
    	randNumbers->at(i) = tempRandomNumber;
    }

}

void RandomGeneratorRANDU::getNormalRandomInt(double mu, double sigma, int* randNumber) {

	double tempRandomNumber = 0;

	getNormalRandomDouble(mu, sigma, &tempRandomNumber);
	*randNumber = int(std::round(tempRandomNumber));
}

void RandomGeneratorRANDU::getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) {

	int tempRandNumber = 0;

	randNumbers->clear();
	randNumbers->resize(size);
	for (std::size_t i=0;i<size;i++){
		getNormalRandomInt(mu, sigma, &tempRandNumber);
		randNumbers->at(i) = tempRandNumber;
	}

}

void RandomGeneratorRANDU::getNormalRandomDouble(double mu, double sigma, double* randNumber) {

	double tempRandom1 = 0;
	double tempRandom2 = 0;
	double tempResult = 0;

	getUniformRandomDouble(0,1,&tempRandom1);
	getUniformRandomDouble(0,1,&tempRandom2);

	tempResult = sqrt(-2*log(tempRandom1))*cos(2*M_PI*tempRandom2); //Box-Mueller transformation to get
	// normal distributed random number
	tempResult = tempResult*sigma + mu; //scale to correct mean and variance

	*randNumber = tempResult;

}

void RandomGeneratorRANDU::getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers, std::size_t size) {

	double tempRandNumber = 0;

	randNumbers->clear();
	randNumbers->resize(size);
	for (std::size_t i=0;i<size;i++){
		getNormalRandomDouble(mu, sigma, &tempRandNumber);
		randNumbers->at(i) = tempRandNumber;
	}

}
