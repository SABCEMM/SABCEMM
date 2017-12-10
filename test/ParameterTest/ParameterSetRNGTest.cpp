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

#include "gtest/gtest.h"

#include "../../src/Parameter/ParameterSetRNG.h"
#include <cstddef> //for std::size_t

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>

using namespace std;
using namespace tinyxml2;


/// @todo test copy constructor
/// @todo checkConsistency besser testen

TEST(ParameterSetRNGTest, poolSize) {


    string filename="ParameterTest/parameterSetRNGTest.xml";

    if(!std::ifstream(filename).good())
    {
        cerr << "Could not open file: "
             << filename << endl;
        exit(1);
    }

    //Load XML file
    XMLDocument doc;
    doc.LoadFile(filename.c_str());

    // Get root element
    XMLElement* root = doc.RootElement(); // <case>

    if(root == NULL)
    {
        cerr << "Could not parse input file - You may want to run xmllint "
             << "on your file to make sure it is valid XML." << endl;
        exit(1);
    }

    // Iterate over all cases
    for (XMLElement* elem = root->FirstChildElement(); elem != nullptr;
         elem = elem->NextSiblingElement()) // <RNGSettings>
    {
        ParameterSetRNG parameterSetRNG = ParameterSetRNG::readFromXML(elem);

        if(*(parameterSetRNG.type) == string("RandomGeneratorVOID1")){
            EXPECT_EQ(*(parameterSetRNG.seed),123);
            EXPECT_EQ(*(parameterSetRNG.poolSizeNormal),688);
            EXPECT_EQ(*(parameterSetRNG.poolSizeUniform),445);
            EXPECT_TRUE(*(parameterSetRNG.enablePool));
            EXPECT_EQ(*(parameterSetRNG.fileName),string("testfile"));
        }else if(*(parameterSetRNG.type) == "RandomGeneratorVOID2"){
            EXPECT_FALSE(parameterSetRNG.seed);
            EXPECT_EQ(*(parameterSetRNG.poolSizeNormal),500);
            EXPECT_EQ(*(parameterSetRNG.poolSizeUniform),500);
            EXPECT_FALSE(parameterSetRNG.fileName);
            EXPECT_TRUE(*(parameterSetRNG.enablePool));
        }else if(*(parameterSetRNG.type) == "RandomGeneratorVOID3"){
            EXPECT_FALSE(parameterSetRNG.seed);
            EXPECT_FALSE(parameterSetRNG.poolSizeNormal);
            EXPECT_FALSE(parameterSetRNG.poolSizeUniform);
            EXPECT_EQ(*(parameterSetRNG.fileName),string("AwesomeFilename"));
            EXPECT_FALSE(*(parameterSetRNG.enablePool));
        } else{
            EXPECT_TRUE(false) << "Testcase unknown: " << *(parameterSetRNG.type);
        }
    }


}