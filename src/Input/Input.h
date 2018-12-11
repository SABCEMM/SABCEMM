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

#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#if BUILD_TESTS
#include "gtest/gtest_prod.h"

#endif

#include <string>
#include <vector>
#include <set>
#include <tinyxml2/tinyxml2.h>


/** Class that defines a parameter object.
 * Contains the parameters that are needed for the simulation. Relies on boost::optionals since not all parameters have to be set for all kinds of simulations.
 */
class Input {
public:
    struct fromFile{
        std::vector<Input> inputs;
        int numThreads = 1;
        std::string filename = "output";
        std::string writerClass = "writertxt";
        std::size_t simulationsPerFile = 1;
        tinyxml2::XMLDocument* doc;
    };
    Input();
    Input(std::string name);
    Input(std::string name_, tinyxml2::XMLElement* el);
    virtual ~Input();
    static fromFile readFromFile(std::string filename);

    bool hasChildren();
    bool hasChild(std::string name);
    Input& getChild(std::string name);
    bool hasValue();

    std::string getString();
    int getInt();
    double getDouble();
    std::string getName();
    size_t getSizeT();
    std::vector<int> getIntVector();
    std::set<int> getIntSet();
    bool getBool();

    void insert(Input insertValue);
    std::vector<Input>& getChildren();

    void addChild(std::string name);
    void addChild(std::string name, tinyxml2::XMLElement* el);

    Input& operator[](std::string name);
    //friend ParameterValue* operator[](ParameterValue& parameterValue, std::string& name);
    bool operator()(std::string name);
    //friend bool operator()(ParameterValue& parameterValue, std::string& name);

private:
    bool check();
    tinyxml2::XMLElement* element;
    std::vector<Input> children;
    std::string name;
    static std::vector<Input> includeXML(std::string folder, tinyxml2::XMLElement* includeElement);
    static void parse(Input& root, tinyxml2::XMLElement* iterElem);
    void checkConversionError(tinyxml2::XMLError error, std::string type);

};



#endif //PROJECT_INPUT_H
