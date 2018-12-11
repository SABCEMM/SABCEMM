/* Copyright 2018 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Cramer, Simon
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
 * @date 17 Apr 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "gtest/gtest.h"

#include "../../src/Input/Input.h"
#include "../../src/Exceptions/ParameterExistanceError.h"
#include "../../src/Exceptions/ParameterConversionError.h"
#include <vector>
#include <iostream>
#include <set>


using namespace std;

TEST(InputTest, singleFile) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    EXPECT_EQ(input.getName(), "case");

    EXPECT_TRUE(input.hasChild("a"));
    EXPECT_TRUE(input.hasChild("b"));
    EXPECT_FALSE(input.hasChild("c"));
    EXPECT_TRUE(input.getChild("b").hasChildren());
    EXPECT_TRUE(input.getChild("b").hasChild("c"));
    EXPECT_TRUE(input.getChild("b").hasChild("d"));
    EXPECT_TRUE(input.hasChild("e"));

    EXPECT_DOUBLE_EQ(input.getChild("a").getDouble(),-7.2);
    EXPECT_EQ(input.getChild("b").getChild("c").getDouble(),6);
    EXPECT_EQ(input.getChild("b").getChild("d").getDouble(),5);
    EXPECT_EQ(input.getChild("e").getString(),"teststring");

    EXPECT_EQ(input.getChild("e").getName(),"e");
}

TEST(InputTest, unknownParameter) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    EXPECT_FALSE(input.hasChild("x"));
    EXPECT_THROW(input.getChild("x").getName(), ParameterExistanceError);
}

TEST(InputTest, includeInCase) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputInclude.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    EXPECT_TRUE(input.hasChild("a"));
    EXPECT_TRUE(input.hasChild("b"));
    EXPECT_FALSE(input.hasChild("c"));
    EXPECT_TRUE(input.getChild("b").hasChildren());
    EXPECT_TRUE(input.getChild("b").hasChild("c"));
    EXPECT_TRUE(input.getChild("b").hasChild("d"));
    EXPECT_TRUE(input.hasChild("e"));

    EXPECT_DOUBLE_EQ(input.getChild("a").getDouble(),7.2);
    EXPECT_EQ(input.getChild("b").getChild("c").getDouble(),6);
    EXPECT_EQ(input.getChild("b").getChild("d").getDouble(),5);
    EXPECT_EQ(input.getChild("e").getString(),"teststring");

    EXPECT_TRUE(input.hasChild("g"));
    EXPECT_DOUBLE_EQ(input.getChild("g").getDouble(),8.2);
    EXPECT_TRUE(input.getChild("b").hasChild("f"));
    EXPECT_EQ(input.getChild("b").getChild("f").getString(),"test");
}

TEST(InputTest, includeMultCase) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputIncludeCase.xml");

    EXPECT_EQ(inputs.inputs.size(),4);
}

TEST(InputTest, operatorTest) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    EXPECT_TRUE(input("a"));
    EXPECT_TRUE(input["b"].hasChildren());
}

TEST(InputTest, getSetInt) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    std::set<int> testSet = input["b"].getIntSet();
    EXPECT_EQ(2, testSet.size());
    EXPECT_EQ(1, testSet.count(5));
    EXPECT_EQ(1, testSet.count(6));

}

TEST(InputTest, getVectorInt) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    std::vector<int> testVector = input["b"].getIntVector();
    EXPECT_EQ(2, testVector.size());
    EXPECT_TRUE(std::find(testVector.begin(), testVector.end(), 5) != testVector.end());
    EXPECT_TRUE(std::find(testVector.begin(), testVector.end(), 6) != testVector.end());
}

TEST(InputTest, doubleName) {
    EXPECT_ANY_THROW(Input::fromFile inputs = Input::readFromFile("InputTest/doubleName.xml"));

}

TEST(InputTest, getString) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    std::string t1 = input["a"].getString();
    std::string t2 = input["l"].getString();
    std::string t3 = input["k"].getString();
    std::string t4 = input["h"].getString();
    std::string t5 = input["i"].getString();
    std::string t6 = input["e"].getString();

    EXPECT_EQ(t1, "-7.2");
    EXPECT_EQ(t2, "8.2");
    EXPECT_EQ(t3, "9");
    EXPECT_EQ(t4, "-10");
    EXPECT_EQ(t5, "false");
    EXPECT_EQ(t6, "teststring");

}

TEST(InputTest, getInt) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    int t1 = input["a"].getInt();
    int t2 = input["l"].getInt();
    int t3 = input["k"].getInt();
    int t4 = input["h"].getInt();
    EXPECT_THROW( input["i"].getInt(), ParameterConversionError);
    EXPECT_THROW( input["e"].getInt(), ParameterConversionError);

    EXPECT_EQ(t1, -7);
    EXPECT_EQ(t2, 8);
    EXPECT_EQ(t3, 9);
    EXPECT_EQ(t4, -10);

}

TEST(InputTest, getDouble) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    double t1 = input["a"].getDouble();
    double t2 = input["l"].getDouble();
    double t3 = input["k"].getDouble();
    double t4 = input["h"].getDouble();
    EXPECT_THROW( input["i"].getDouble(), ParameterConversionError);
    EXPECT_THROW( input["e"].getDouble(), ParameterConversionError);

    EXPECT_DOUBLE_EQ(t1, -7.2);
    EXPECT_DOUBLE_EQ(t2, 8.2);
    EXPECT_DOUBLE_EQ(t3, 9);
    EXPECT_DOUBLE_EQ(t4, -10);

}

TEST(InputTest, getSizeT) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    EXPECT_THROW( input["a"].getSizeT(), ParameterConversionError);
    size_t t2 = input["l"].getSizeT();
    size_t t3 = input["k"].getSizeT();
    EXPECT_THROW( input["h"].getSizeT(), ParameterConversionError);
    EXPECT_THROW( input["i"].getSizeT(), ParameterConversionError);
    EXPECT_THROW( input["e"].getSizeT(), ParameterConversionError);

    EXPECT_EQ(t2, 8);
    EXPECT_EQ(t3, 9);

}

TEST(InputTest, getBool) {
    Input::fromFile inputs = Input::readFromFile("InputTest/inputSingle.xml");

    EXPECT_EQ(inputs.inputs.size(),1);

    Input input = inputs.inputs.at(0);
    EXPECT_TRUE(input.hasChildren());

    bool t1 = input["a"].getBool() ;
    bool t2 = input["l"].getBool() ;
    bool t3 = input["k"].getBool() ;
    bool t4 = input["h"].getBool() ;
    bool t5 = input["i"].getBool();
    EXPECT_THROW( input["e"].getBool(), ParameterConversionError);

    EXPECT_EQ(t1, true);
    EXPECT_EQ(t2, true);
    EXPECT_EQ(t3, true);
    EXPECT_EQ(t4, true);
    EXPECT_EQ(t5, false);

}
