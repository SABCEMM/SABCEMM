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

#include "RandomGeneratorFromFile.h"
#include <fstream>
#include <boost/core/ignore_unused.hpp>
#include <string>
#include <exception>

using namespace std;

RandomGeneratorFromFile::RandomGeneratorFromFile(std::string filename)
    :RandomGenerator(-1)
{
    ifstream ifs(filename);

    if(!ifs.good())
        throw runtime_error(string("Could not open file: ") + filename);

    while(ifs.good())
    {
        double num;

        ifs >> num;
        numbers.push_back(num);
    }
}

void RandomGeneratorFromFile::getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) {
    boost::ignore_unused(lowerBound, upperBound);
    *randNumber = (int)numbers.at(0);
    numbers.pop_front();
}

void RandomGeneratorFromFile::getUniformRandomInt(int lowerBound, int upperBound, std::vector<int> *randNumbers,
                                                  std::size_t size) {
    boost::ignore_unused(lowerBound, upperBound);
    randNumbers->clear();
	randNumbers->resize(size);
	for(std::size_t i=0;i<size;i++)
	{
        (*randNumbers)[i] = (int)numbers.at(0);
        numbers.pop_front();
	}
}

void RandomGeneratorFromFile::getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) {
    boost::ignore_unused(lowerBound, upperBound);
    *randNumber = numbers.at(0);
    numbers.pop_front();
}

void RandomGeneratorFromFile::getUniformRandomDouble(double lowerBound, double upperBound,
                                                     std::vector<double> *randNumbers, std::size_t size) {
    boost::ignore_unused(lowerBound, upperBound);
    randNumbers->clear();
	randNumbers->resize(size);
    for(std::size_t i=0;i<size;i++)
    {
        (*randNumbers)[i] = numbers.at(0);
        numbers.pop_front();
    }
}


void RandomGeneratorFromFile::getNormalRandomInt(double mu, double sigma, int* randNumber) {
    boost::ignore_unused(mu, sigma);
    *randNumber = (int)numbers.at(0);
    numbers.pop_front();
}

void RandomGeneratorFromFile::getNormalRandomInt(double mu, double sigma, std::vector<int> *randNumbers, std::size_t size) {
    boost::ignore_unused(mu, sigma);
    randNumbers->clear();
    randNumbers->resize(size);
    for(std::size_t i=0;i<size;i++)
    {
        (*randNumbers)[i] = (int)numbers.at(0);
        numbers.pop_front();
    }
}

void RandomGeneratorFromFile::getNormalRandomDouble(double mu, double sigma, double* randNumber) {
    boost::ignore_unused(mu, sigma);
    *randNumber = numbers.at(0);
    numbers.pop_front();
}

void RandomGeneratorFromFile::getNormalRandomDouble(double mu, double sigma, std::vector<double> *randNumbers,
                                                    std::size_t size) {
    boost::ignore_unused(mu, sigma);
    randNumbers->clear();
    randNumbers->resize(size);
    for(std::size_t i=0;i<size;i++)
    {
        (*randNumbers)[i] = numbers.at(0);
        numbers.pop_front();
    }
}
