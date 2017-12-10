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

#include <iostream>
#include <fstream>
#include <chrono>

#include "RandomGenerator.h"
#include "RandomGeneratorStdLib.h"
#include "../Writer/Writer.h"
#if WITH_INTEL_MKL
#include "RandomGeneratorIntelMKL.h"

#endif
#if WITH_NAG
#include "RandomGeneratorNAG.h"
#endif
#include "RandomGeneratorStdLibOld.h"
#include "RandomGeneratorRANDU.h"
#include "RandomGeneratorFromFile.h"
#include "RandomNumberPool.h"

using namespace std;

RandomGenerator::RandomGenerator(int seed): seed(seed)
{

}

RandomGenerator* RandomGenerator::factory(Parameter* parameter, Writer* writer){
	optional<string> type = parameter->parameterSetRNG.type;
	optional<int> seed = parameter->parameterSetRNG.seed;
	optional<string> fileName = parameter->parameterSetRNG.fileName;


	RandomGenerator* randomGenerator;
	//RandomGenerator
	if (*type == "RandomGeneratorStdLib") {
		if(seed){
			randomGenerator = new RandomGeneratorStdLib(*seed);
		}
		else{
			randomGenerator = new RandomGeneratorStdLib();
		}
	}
	else if (*type == "RandomGeneratorStdLibOld") {
		if(seed){
			randomGenerator = new RandomGeneratorStdLibOld(*seed);
		}
		else{
			randomGenerator = new RandomGeneratorStdLibOld();
		}
	}
#if WITH_INTEL_MKL
	else if (*type== "RandomGeneratorIntelMKL") {

		if(seed){
			randomGenerator = new RandomGeneratorIntelMKL(*seed);
		}
		else{
			randomGenerator = new RandomGeneratorIntelMKL();
		}
	}
#endif
#if WITH_NAG
		else if (*type == "RandomGeneratorNAG") {
		if(seed){
			randomGenerator = new RandomGeneratorNAG(*seed);
		}
		else{
			randomGenerator = new RandomGeneratorNAG();
		}
	}
#endif
	else if (*type == "RandomGeneratorRANDU") {
		if(seed){
			randomGenerator = new RandomGeneratorRANDU(*seed);
		}
		else{
			randomGenerator = new RandomGeneratorRANDU();
		}
	}
	else if (*type == "RandomGeneratorFromFile") {
		if(fileName){
			randomGenerator = new RandomGeneratorFromFile(*fileName);
		}
		else
			throw("RandomGeneratorFromFile needs a file name.");
	}
	else {
		throw("RandomGeneratorClass unknown!");
	}
	//RandomNumberPool TODO: Make nice!!
	if (*(parameter->parameterSetRNG.enablePool)) {
		RandomNumberPool* randomNumberPool = new RandomNumberPool(*(parameter->parameterSetRNG.poolSizeUniform),
																  *(parameter->parameterSetRNG.poolSizeNormal));
		randomNumberPool->setRandomGenerator(randomGenerator);
		randomNumberPool->setWriter(writer);
		return dynamic_cast<RandomGenerator*>(randomNumberPool);
	} else {
		return dynamic_cast<RandomGenerator*>(randomGenerator);
	}

}


int RandomGenerator::createSeed() {
	ifstream randomdevice;
	int seed = 0;

	try {
        char c, d, e, f;

		randomdevice.open("/dev/urandom", ios::in);

		randomdevice >> c;
		randomdevice >> d;
		randomdevice >> e;
		randomdevice >> f;
        seed = (int) c * (int) d + (int) e * (int) f;

		randomdevice.close();
	}

	catch (...) {
		auto time = std::chrono::high_resolution_clock::now();
		seed = static_cast<int>(time.time_since_epoch().count());
		cerr << " RandomDevice not availiable. Current time is choose as seed: " << seed << endl;
	}

	return seed;
}


