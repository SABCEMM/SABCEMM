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

#include <string>
#include <vector>
#include "gmock/gmock.h"

#include "../../src/Writer/Writer.h"
#include "../../src/Parameter/Parameter.h"
#include <cstddef> //for std::size_t
class MockWriter: public Writer{
public:
    MOCK_METHOD1(saveInput, void(Parameter* newParameter));
    MOCK_METHOD0(saveBuildInfo,void());
    MOCK_METHOD2(vectorToFile, void(std::vector<double>* newVector, std::string name));
    MOCK_METHOD3(vectorToFile, void(std::vector<double>* newVector, std::string name, int groupID));
    MOCK_METHOD4(vectorToFile, void(std::vector<double>* newVector, std::string name, int groupID, Util::DataItemCollectorMethod method));
    MOCK_METHOD4(arrayToFile, void(double* newArray, int arraySize, std::string name, int groupID));
    MOCK_METHOD7(rngInformation, void(std::size_t uniformPoolInitialSize, std::size_t uniformPoolSize, int uniformPoolRefills,
            std::size_t normalPoolInitialSize, std::size_t normalPoolSize, int normalPoolRefills, int seed));
    MOCK_METHOD2(matrixToFile, void(std::vector<std::vector<double>>* newVector, std::string name));
    MOCK_METHOD3(matrixToFile, void(std::vector<std::vector<double>>* newVector, std::string name, int groupID));
    MOCK_METHOD4(matrixToFile, void(std::vector<std::vector<double>>* newVector, std::string name, int groupID, Util::DataItemCollectorMethod method));
    MOCK_METHOD1(saveTime, void(double time));
};

