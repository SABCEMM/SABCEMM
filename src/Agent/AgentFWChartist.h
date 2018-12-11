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
 * @file AgentEMB.h
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef PROJECT_AGENTFWCHARTIST_H
#define PROJECT_AGENTFWCHARTIST_H

#if BUILD_TESTS
//Necessary include to test private functions with googleTest
#include "gtest/gtest_prod.h"
#endif

#include "AgentFW.h"
#include <string>

/// @ingroup Franke-Westerhoff
class AgentFWChartist : public AgentFW {
#if BUILD_TESTS
    FRIEND_TEST(fullSimulationTest, fullSimulation_DCA_HPM);
    FRIEND_TEST(fullSimulationTest, fullSimulation_TPA_W);
    FRIEND_TEST(fullSimulationTest, fullSimulation_TPAC_W);
#endif
protected:
    /// inflation factor for chartist demand
    const double chi;
public:
    void stepUpdate() override;
    double calculateContributedAttractiveness() override;
    AgentFWChartist(RandomGenerator* randomGenerator, Price* price, double eta, double beta, double alpha_w, double alpha_n,
                  double alpha_p, double alpha_0, double nu, SwitchingStrategy switchingStrategy, std::string indexStrategy,
                  double sigma, double chi);

public:
    double calculateDemand() override;

};


#endif //PROJECT_AGENTFWCHARTIST_H
