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
 * @author Cramer
 * @date 11 Jul 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */
#include <cstddef> //for std::size_t
#include "gtest/gtest.h"

#include "../src/QuantitiesOfInterest/QoiCalculator.h"
#include "../src/QuantitiesOfInterest/QuantityOfInterest.h"
#include "../MockClasses/MockQoiCalculator.h"

TEST(QoiCalculator, getMethod){
MockQoiCalculator qoiCalculator;
qoiCalculator.method = QuantityOfInterest::Method::full;

ASSERT_EQ(QuantityOfInterest::Method::full, qoiCalculator.getMethod());
}

TEST(QoiCalculator, setMethod){
MockQoiCalculator qoiCalculator;
qoiCalculator.method = QuantityOfInterest::Method::full;
qoiCalculator.setMethod(QuantityOfInterest::Method::autocorrelation);
    ASSERT_EQ(QuantityOfInterest::Method::autocorrelation, qoiCalculator.getMethod());
    ASSERT_EQ(QuantityOfInterest::Method::autocorrelation, qoiCalculator.method);
}


TEST(QoiCalculator, getResult){
    MockQoiCalculator qoiCalculator;
    std::vector<std::vector<double>> result;

    qoiCalculator.result = result;

    ASSERT_EQ(qoiCalculator.result, *qoiCalculator.getResult());
}





