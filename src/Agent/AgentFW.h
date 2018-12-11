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



#include <string>

/// @defgroup Franke-Westerhoff
/// agents and calculators for Franke-Westerhoff

/// @ingroup Franke-Westerhoff
/// Abstract base class for Franke-Westerhoff agents
///
/// Control flow remains in AgentFW while specializations define demand and
/// share calculation.
///
/// In the spirit of the original model, every two @ref AgentFundamentalist
/// and @ref AgentChartist are simulated together. They need to be coupled to
/// allow for the calculation of relative @ref attractiveness. Coupling is
/// realized via the @ref partner member variable.
class AgentFW : public Agent {
#if BUILD_TESTS
    FRIEND_TEST(fullSimulationTest, fullSimulation_DCA_HPM);
    FRIEND_TEST(fullSimulationTest, fullSimulation_TPA_W);
    FRIEND_TEST(fullSimulationTest, fullSimulation_TPAC_W);
#endif
public:

    /// Determines how to calculate fundamentalist share from switching index
/// See ch.3.2 in the paper.
/// @ingroup Franke-Westerhoff
/// @see stringToSwitchingStrategy
    enum SwitchingStrategy {
        /// discrete choice approach
                DCA = 0,
        /// transition probability approach
                TPA,
        /// transition probability approach continous
                TPAC,
        /// transitions probability approach continous implicit (Haja S.22)
                TPACI
    };

/// Converts a string to the corresponding SwitchingStrategy.
/// @todo gibt es keine bessere Lösung für string ==> enum?
    static std::map<std::string, SwitchingStrategy> stringToSwitchingStrategy;

/// Determines how to calculate the switching index
/// See p.11, ch.3.3 in the paper.
/// @ingroup Franke-Westerhoff
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

/// Holds information about which index strategies are turned on.
/// The indices of the bitset correspond to the numbers in IndexStrategy.
/// @see IndexStrategy
    typedef std::bitset<4> IndexStrategies;

    /// Constructs an AgentFW. A partner must be specified before use.
    /// @see setPartner
    AgentFW(RandomGenerator* randomGenerator, Price* price, double eta, double beta, double alpha_w, double alpha_n,
            double alpha_p, double alpha_0, double nu, AgentFW::SwitchingStrategy switchingStrategy, std::string indexStrategy,
            double sigma);

protected:
    /// @name DCA variables
    //@{
    /// memory coefficient
    const double eta;
    /// intensity of choice
    const double beta;
    //@}

    /// @name factors to relative attractiveness
    //@{
    /// influence of wealth
    const double alpha_w;
    /// influence of herding
    const double alpha_n;
    /// influence of misalignment
    const double alpha_p;
    /// influence of predisposition
    const double alpha_0;
    //@}

    /// @name TPA variables
    //@{
    /// flexibility parameter
    const double nu;
    //@}

    const SwitchingStrategy switchingStrategy;
    /// the bits represent the activation of various terms in the calculation of relative attraciveness
    /// for the mapping of index <-> strategy, @see IndexStrategy.
    IndexStrategies indexStrategy;

    /// @name noise
    //@{
    /// standard deviation for eps. It holds: eps ~ N(0, sigma)
    const double sigma;
    /// current demand noise. Used by implementations to add to the deterministic demand.
    double eps;
    //@}

    /// price at time t-1 during stepUpdate
    double oldPrice;

    /// short-term capital gains
    double g;
    /// hypothetical wealth
    double w;

    /// relative attractiveness
    double attractiveness;
    /// relative attractiveness at time t-1.
    double oldAttractiveness;

    /// demand
    double d;
    /// demand at time t-1
    double oldd;
    /// demand at time t-2
    double oldoldd;

    /// share
    double n;
    /// share at time t-1
    double oldn;
private:
    AgentFW* partner;
public:
    /// Populates the memory variables ("old" variables).
    void preStepUpdate();
    /// Updates d, g, w.
    void stepUpdate();
    /// Updates a_t, then the share and trading volume.
    /// @note we must split the update step as @ref attractiveness needs an
    /// up-to-date w of both agents.
    void postStepUpdate();

    /// Simple setter.
    /// @throw partners cannot be reassigned. This method will throw, if you try to.
    void setPartner(AgentFW *partner) {
        if(this->partner != nullptr)
            throw "AgentFW already has a partner";
        this->partner = partner;
    }

    /// Simple getter.
    double getShare(){
        return n;
    }

    void setShare(double n_);

    /// Calculates agent @ref d "demand".
    virtual double calculateDemand() = 0;
    /// @ref attractiveness is linearly composed of three terms, of which two can
    /// be exclusively calculated by the chartist and the fundamentalist,
    /// respectively. They should implement this method to do so.
    virtual double calculateContributedAttractiveness() = 0;

    /// @todo it's unclear what an update with hypothetical prices means to
    ///       this agent.
    void updateBisection(const double&){
        throw "not implemented.";
    }

    SwitchingStrategy getSwitchingStrategy(){
        return switchingStrategy;
    }
};

#endif // AGENTFW_H
