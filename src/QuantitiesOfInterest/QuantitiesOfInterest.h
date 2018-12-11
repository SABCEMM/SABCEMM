/* Copyright 2017 - BSD-3-Clause
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
 * @file QuantitiesOfInterest.h
 * @author Cramer
 * @date 11 May 2018
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef PROJECT_QUANTITIESOFINTEREST_H
#define PROJECT_QUANTITIESOFINTEREST_H

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

#include <vector>
#include "QuantityOfInterest.h"
#include "../Input/Input.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../Agent/Agent.h"
#include "../Writer/Writer.h"
//#include "../DataCollector/DataCollector.h"
class DataCollector;

class QuantitiesOfInterest {
#if BUILD_TESTS
    //FRIEND_TEST(**, **);
#endif
public:
    QuantitiesOfInterest();
    ~QuantitiesOfInterest();
    static QuantitiesOfInterest* factory(Input &input, Price *price, ExcessDemand *excessDemand,
                                                        std::vector<Agent *> *agents,
                                                        std::vector<Switchable *> &switchableGroups,
                                                    DataCollector* dataCollector);
    void addQOI(QuantityOfInterest* quantityOfInterest);
    void calculateQoi();

    void write(Writer* writer);

private:
    std::vector<QuantityOfInterest*> quantitiesOfInterest;


};


#endif //PROJECT_QUANTITIESOFINTEREST_H
