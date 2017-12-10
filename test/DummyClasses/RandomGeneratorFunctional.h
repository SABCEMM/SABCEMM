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

#ifndef RANDOMGENERATORFUNCTIONAL_H_
#define RANDOMGENERATORFUNCTIONAL_H_

#include <cstddef> //for std::size_t
#include "../../src/RandomGenerator/RandomGenerator.h"
#include <boost/core/ignore_unused.hpp>
#include <functional>

/// Draw constant numbers.
class ConstantReturn{
public:
    explicit ConstantReturn(double constantReturn):
        constantReturn(constantReturn) { }

    double operator()(std::size_t count) const
    {
        boost::ignore_unused(count);
        return constantReturn;
    }

private:
    double constantReturn;
};

/// Draw numbers from a list.
class ListReturn{
public:
    explicit ListReturn(const std::vector<double>& numbers):
        numbers(numbers) { }

    double operator()(std::size_t count)
    {
        return numbers.at(count-1);
    }

private:
    std::vector<double> numbers;
};


class RandomGeneratorFunctional: public RandomGenerator{
public:
    explicit RandomGeneratorFunctional(const std::function<double(std::size_t)> &returnFunction);

    void getUniformRandomInt(int lowerBound, int upperBound, int* randNumber) ;
    void getUniformRandomInt(int lowerBound, int upperBound, std::vector<int>* randNumbers, std::size_t size) ;
    void getUniformRandomDouble(double lowerBound, double upperBound, double* randNumber) ;
    void getUniformRandomDouble(double lowerBound, double upperBound, std::vector<double>* randNumbers, std::size_t size) ;

    void getNormalRandomInt(double mu, double sigma, int* randNumber) ;
    void getNormalRandomInt(double mu, double sigma, std::vector<int>* randNumbers, std::size_t size) ;
    void getNormalRandomDouble(double mu, double sigma, double* randNumber) ;
    void getNormalRandomDouble(double mu, double sigma, std::vector<double>* randNumbers, std::size_t size) ;
private:
    std::function<double(std::size_t)> returnFunction;
    /// for each get-function, count is increased by one.
    std::size_t count;
};

#endif /* RANDOMGENERATORFUNCTIONAL_H_ */
