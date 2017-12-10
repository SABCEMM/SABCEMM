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

#ifndef AGENTFW_H
#define AGENTFW_H

// referenced paper: Structural Stochastic Volatility in Asset Pricing Dynamics: Estimation and Model Contest

#include "Agent.h"
#include <boost/assign.hpp>
#include <map>
#include <bitset>

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

/// Determines how to calculate fundamentalist share from switching index
/// See ch. 3.2 in the paper.
enum SwitchingStrategy {
    /// discrete choice approach
    DCA = 0,
    /// transition probability approach
    TPA
};

/// @todo gibt es keine bessere Lösung für string ==> enum?
const std::map<std::string, SwitchingStrategy> switchingStrategyToString =
        boost::assign::map_list_of("DCA", DCA)("TPA", TPA);

/// Determines how to calculate the switching index
/// See p.11, ch. 3.3 in the paper.
enum IndexStrategy
{
    /// wealth
    W=0,
    /// herding
    H=1,
    /// predisposition
    P=2,
    /// misalignment
    M=3
};

typedef std::bitset<4> IndexStrategies;

class AgentFW : public Agent {
#if BUILD_TESTS
FRIEND_TEST(fullSimulationTest, fullSimulation_FW);
#endif
public:
    AgentFW(RandomGenerator* randomGenerator, Price* price, double eta, double beta, double alpha_w, double alpha_n,
            double alpha_p, double alpha_0, double nu, SwitchingStrategy switchingStrategy, const IndexStrategies& indexStrategy,
            double sigma);

protected:
    /// price at time t-1 during stepUpdate
    double oldPrice;
    /// demand at time t-1 during stepUpdate
    /// note: demand is equivalent to tradingVolume here.
    double oldDemand;
    /// demand at time t-2 during stepUpdate
    /// note: demand is equivalent to tradingVolume here.
    double oldoldDemand;

    /// short-term capital gains
    double g;
    /// hypothetical wealth
    double w;

    /// relative attractiveness
    double attractiveness;
    /// relative attractiveness at time t-1.
    double oldAttractiveness;

    /// memory coefficient
    const double eta;
    /// intensity of choice
    const double beta;

    /// influence of wealth
    const double alpha_w;
    const double alpha_n;
    const double alpha_p;
    const double alpha_0;

    const double nu;

    const SwitchingStrategy switchingStrategy;
    /// the bits represent the activation of various terms in the calculation of relative attraciveness
    /// for the mapping of index <-> strategy, @see IndexStrategy.
    const IndexStrategies indexStrategy;

    // demand
    double d;
    /// demand at time t-1
    double oldd;
    /// demand at time t-2
    double oldoldd;
    const double sigma;
    /// current demand noise
    double eps;

    /// share
    double n;
    /// share at time t-1
    double oldn;
private:
    AgentFW* partner;
public:
    // update g_s, w_s (hypothetical wealth)
    void preStepUpdate();
    void stepUpdate();
    void postStepUpdate();

    void setPartner(AgentFW *partner) {
        if(this->partner != nullptr)
            throw "AgentFW already has a partner";
        this->partner = partner;
    }

    double getShare(){
        return n;
    }

    virtual double calculateDemand() = 0;
    /// @todo schöner ware es, den nur einmal zu berechnen! Scheitert momentan am Reihenfolgenproblem.
    virtual double calculateContributedAttractiveness() = 0;

    void updateBisection(const double&){};
};

#endif // AGENTFW_H
