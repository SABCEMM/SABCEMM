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
#include <cstdio>
#include "mkl.h"

#include <cmath>
#include "RandomGeneratorIntelMKL.h"
#include <cassert>
#include <iostream>

using namespace std;





/** Standard constructor. Uses Seed from RandomDevice
 *
 */
RandomGeneratorIntelMKL::RandomGeneratorIntelMKL():RandomGeneratorIntelMKL(createSeed()) {
}

/** Constructor that uses a given seed
 * \param seed Seed for the randomGenerator
 */
RandomGeneratorIntelMKL::RandomGeneratorIntelMKL(int seed): RandomGenerator(seed) {

	//Stream fuer Zufallszahlen erstellen, Seed aus Uebergabe
	//VSL_BRNG_MT2203
	//VSL_BRNG_MT19937
	//VSL_BRNG_NONDETERM
	//VSL_BRNG_ARS5
	//VSL_BRNG_PHILOX4X32X10
	vslNewStream(&stream, VSL_BRNG_MT2203, static_cast<const unsigned int>(seed));
}


/** Standard destructor.
 *
 */
RandomGeneratorIntelMKL::~RandomGeneratorIntelMKL() {
	vslDeleteStream( &stream);
}


/// @copydoc RandomGenerator::getUniformRandomInt(int, int, int*)
/// @todo diese Funktion gibt Zahlen aus dem Interval [a,b) zurueck!!
void RandomGeneratorIntelMKL::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber){
	// This functions returns random numbers from the interval [lowerBound, upperBound). Therefore upperBound is raised by 1.
	viRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 1, randNumber, lowerBound, upperBound );
}

void RandomGeneratorIntelMKL::getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers,
                                                  std::size_t size){
	//int tempRandNumbers[size];
	randNumbers->clear();
	randNumbers->resize(size);
	// This functions returns random numbers from the interval [lowerBound, upperBound). Therefore upperBound is raised by 1.
	//viRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, size, tempRandNumbers , lowerBound, upperBound+1);
	viRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, (int)size, randNumbers->data() , lowerBound, upperBound+1);
	//for (int i=0; i<size; i++)
	//{
	//	randNumbers->at(i) = tempRandNumbers[i];
	//}
}

void RandomGeneratorIntelMKL::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {
	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE, stream, 1, randNumber, lowerBound, upperBound );
}

void RandomGeneratorIntelMKL::getUniformRandomDouble(double lowerBound, double upperBound,
													 std::vector<double> *randNumbers, std::size_t size) {
	//double tempRandNumbers[size];
	randNumbers->clear();
	randNumbers->resize(size);

	//vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE, stream, size, tempRandNumbers, lowerBound, upperBound );
	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE, stream, (int)size, randNumbers->data(), lowerBound, upperBound );
	//for (int i=0; i<size; i++)
	//{
	//	randNumbers->at(i) = tempRandNumbers[i];
	//}
}

void RandomGeneratorIntelMKL::getNormalRandomInt(double mu, double sigma, int* randNumber) {
	double tempRand;
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, 1, &tempRand, mu, sigma);
	*randNumber = int(std::round(tempRand));
}

void RandomGeneratorIntelMKL::getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) {
	double* tempRandNumbers = new double[size];
	randNumbers->clear();
	randNumbers->resize(size);

	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, (int)size, tempRandNumbers, mu, sigma);

	for (std::size_t i=0; i<size; i++)
	{
		randNumbers->at(i) = int(std::round(tempRandNumbers[i]));
	}

	delete[] tempRandNumbers;
	tempRandNumbers = nullptr;
}

void RandomGeneratorIntelMKL::getNormalRandomDouble(double mu, double sigma, double* randNumber) {
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, 1, randNumber, mu, sigma);
}

void RandomGeneratorIntelMKL::getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers,
													std::size_t size) {
	//double* tempRandNumbers;
	//tempRandNumbers = new double[size];
	randNumbers->clear();
	randNumbers->resize(size);

	//vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, size, tempRandNumbers, mu, sigma);
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, static_cast<int>(size), randNumbers->data(), mu, sigma);
}
