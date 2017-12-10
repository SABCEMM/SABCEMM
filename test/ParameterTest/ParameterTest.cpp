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

#include "../../src/Parameter/Parameter.h"
#include <cstddef> //for std::size_t
using namespace std;


/// @todo test copy constructor
/// @todo checkConsistency besser testen

TEST(ParameterTest, empty)
{
    vector<Parameter*> parameters = Parameter::readFromFile("ParameterTest/emptyInput.xml");
    EXPECT_EQ(parameters.size(), 1);
    Parameter* parameter = parameters.at(0);

    EXPECT_EQ(parameter->validate(), false);
}

// A minimal set of parameters
TEST(ParameterTest, minimalTest){
    // tests are run from the Test-folder (which contains the ParameterTest-folder)
    vector<Parameter*> parameters = Parameter::readFromFile("ParameterTest/minimalInput.xml");
    EXPECT_EQ(parameters.size(), 1);
    Parameter* parameter = parameters.at(0);

    ASSERT_TRUE(parameter->validate());

    // see minimalInput.xml
    EXPECT_EQ(*(parameter->parameterSetRNG.type),string("RandomGeneratorVOID"));

    EXPECT_EQ(parameter->parameterSetAgent.size(),1);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).type),string("AgentVOID"));
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).count), 55);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).cash), 5.5);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).stock), 6);

    EXPECT_EQ(*(parameter->numSteps), 12);
    EXPECT_EQ(*(parameter->outputname),string("outVOID_1"));
    EXPECT_EQ(*(parameter->deltaT), 0.1);
    EXPECT_EQ(*(parameter->writerClass),string("WriterVOID"));
    EXPECT_EQ(*(parameter->parameterSetExcessDemandCalculator.excessDemandCalculatorClass),string("ExcessDemandCalculatorVOID"));
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.type),string("PriceCalculatorVOID"));
    EXPECT_EQ(*(parameter->startPrice), 0.5);
}

// A maximum set of parameters
TEST(ParameterTest, maximalTest){
    // tests are run from the Test-folder (which contains the ParameterTest-folder)
    vector<Parameter*> parameters = Parameter::readFromFile("ParameterTest/maximalInput.xml");
    EXPECT_EQ(parameters.size(), 1);
    // <case> 1
    Parameter* parameter = parameters.at(0);
    ASSERT_TRUE(parameter->validate());

    // see maximalInput.xml

    //RNG Settings
    EXPECT_EQ(*(parameter->parameterSetRNG.type),string("RandomGeneratorVOID"));
    EXPECT_EQ(*(parameter->parameterSetRNG.seed),1887470);
    EXPECT_EQ(*(parameter->parameterSetRNG.poolSizeNormal),300);
    EXPECT_EQ(*(parameter->parameterSetRNG.poolSizeUniform),200);
    EXPECT_EQ(*(parameter->parameterSetRNG.fileName),string("AwesomeFilename"));

    //Agent Settings
    EXPECT_EQ(parameter->parameterSetAgent.size(),1);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).type),string("AgentVOID"));
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).count), 2500);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).cash), 5.5);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).stock), 6);

    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).C1), 1.1);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).C2), 2.2);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).C3), 3.3);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).threshold), 4.4);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).g), 5.5);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).alpha), 6.6);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).neighbourhoodGeneratorClass), string("NeighbourhoodGeneratorVOID"));

    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).A1), 7.7);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).A2), 8.8);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).b1), 9.9);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).b2), 1.1);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).gamma), 1.5);

    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).stdNoiseSigma), 2.2);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).riskTolerance), 3.3);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).memorySpan), 4);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).k), 5.5);

    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).historyMean), 7.7);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).historySigma), 8.8);
    EXPECT_EQ(*(parameter->parameterSetAgent.at(0).initialGamma), 9.9);

    EXPECT_EQ(parameter->parameterSetAgent.at(0).groups->size(), 2);
    /// @todo Gibt es hierfür keine bessere Möglichkeit?
    for(vector<int>::size_type i=0;i<parameter->parameterSetAgent.at(0).groups->size();i++){
        if (parameter->parameterSetAgent.at(0).groups->at(i) == 2)
            EXPECT_EQ(parameter->parameterSetAgent.at(0).groups->at(i), 2);
        else if (parameter->parameterSetAgent.at(0).groups->at(i) == 3)
            EXPECT_EQ(parameter->parameterSetAgent.at(0).groups->at(i), 3);
        else
            EXPECT_EQ(0, 1);
    }


    //DataItemCollector Settings
    EXPECT_EQ(parameter->parameterSetDataItemCollector.size(), 1);
    EXPECT_EQ(*(parameter->parameterSetDataItemCollector.at(0).type), string("DataCollectorVOID"));
    EXPECT_EQ(*(parameter->parameterSetDataItemCollector.at(0).collectInterval), 6);
    EXPECT_EQ(*(parameter->parameterSetDataItemCollector.at(0).writeInterval), 7);
    EXPECT_EQ(*(parameter->parameterSetDataItemCollector.at(0).groupToTrack), 5);

    // General Settings
    EXPECT_EQ(*(parameter->numSteps), 8);
    EXPECT_EQ(*(parameter->outputname),string("outVOID_1"));
    EXPECT_EQ(*(parameter->deltaT), 9.9);
    EXPECT_EQ(*(parameter->writerClass),string("WriterVOID"));
    EXPECT_EQ(*(parameter->parameterSetExcessDemandCalculator.excessDemandCalculatorClass),string("ExcessDemandCalculatorVOID"));


    // PriceCalculator Settings
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.type),string("PriceCalculatorVOID"));
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.epsilon), 1.1);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.maxIterations), 2);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.lowerBound), 3.3);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.upperBound), 4.4);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.theta), 5.5);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.fFunction), string("fVOID"));
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.fConstant), 6.6);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.gFunction), string("gVOID"));
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.gConstant), 7.7);
    EXPECT_EQ(*(parameter->parameterSetPriceCalculator.marketDepth), 8.8);

    EXPECT_EQ(*(parameter->startPrice), 20);

    // Dividend Settings
    EXPECT_EQ(*(parameter->parameterSetDividend.Z1), 9.9);
    EXPECT_EQ(*(parameter->parameterSetDividend.Z2), 1.1);
    EXPECT_EQ(*(parameter->parameterSetDividend.interestRate), 2.2);
    EXPECT_EQ(*(parameter->parameterSetDividend.initialDividend), 3.3);

}

TEST(ParameterTest, caseTests){
    // Strongly based on minimalTest
    vector<Parameter*> parameters = Parameter::readFromFile("ParameterTest/caseInput.xml");
    EXPECT_EQ(parameters.size(), 2);
    for(std::size_t i = 0; i < parameters.size(); ++i)
    {
        // only the most important parameters are tested, the focus is on the
        // iterations.
        Parameter* parameter = parameters.at(i);
        ASSERT_TRUE(parameter->validate());

        EXPECT_EQ(*(parameter->parameterSetRNG.type),string("RandomGeneratorVOID"));

        EXPECT_EQ(parameter->parameterSetAgent.size(),1);
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).type),string("AgentVOID"));
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).count), 55);
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).cash), 5.5);
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).stock), 6);

        EXPECT_EQ(*(parameter->numSteps), 12);
        // @TODO: Das sieht so nicht richtig aus ... Andererseits steht die Reihenfolge nicht fest.
        if (*(parameter->outputname) == "case1_1"){
            EXPECT_EQ(*(parameter->outputname),string("case1_1"));
        }
        else if (*(parameter->outputname) == "case2_1"){
            EXPECT_EQ(*(parameter->outputname),string("case2_1"));
        } else{
            EXPECT_TRUE(false);
        }
        EXPECT_EQ(*(parameter->deltaT), 0.1);
        EXPECT_EQ(*(parameter->writerClass),string("WriterVOID"));
        EXPECT_EQ(*(parameter->parameterSetExcessDemandCalculator.excessDemandCalculatorClass),string("ExcessDemandCalculatorVOID"));
        EXPECT_EQ(*(parameter->parameterSetPriceCalculator.type),string("PriceCalculatorVOID"));
        EXPECT_EQ(*(parameter->startPrice), 0.5);

    }
}

TEST(ParameterTest, repetitionsTests){
    // Strongly based on minimalTest
    vector<Parameter*> parameters = Parameter::readFromFile("ParameterTest/repetitionInput.xml");
    EXPECT_EQ(parameters.size(), 10);
    for(std::size_t i = 0; i < parameters.size(); ++i)
    {
        // only the most important parameters are tested, the focus is on the
        // iterations.
        Parameter* parameter = parameters.at(i);
        ASSERT_TRUE(parameter->validate());

        EXPECT_EQ(*(parameter->parameterSetRNG.type),string("RandomGeneratorVOID"));

        EXPECT_EQ(parameter->parameterSetAgent.size(),1);
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).type),string("AgentVOID"));
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).count), 55);
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).cash), 5.5);
        EXPECT_EQ(*(parameter->parameterSetAgent.at(0).stock), 6);

        EXPECT_EQ(*(parameter->numSteps), 12);
        string outname = "outVOID_";
        outname += to_string(i+1);
        EXPECT_EQ(*(parameter->outputname),outname);
        EXPECT_EQ(*(parameter->deltaT), 0.1);
        EXPECT_EQ(*(parameter->writerClass),string("WriterVOID"));
        EXPECT_EQ(*(parameter->parameterSetExcessDemandCalculator.excessDemandCalculatorClass),string("ExcessDemandCalculatorVOID"));
        EXPECT_EQ(*(parameter->parameterSetPriceCalculator.type),string("PriceCalculatorVOID"));
        EXPECT_EQ(*(parameter->startPrice), 0.5);

    }
}
