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

#include "AgentFW.h"
#include <iostream>
using namespace std;

AgentFW::AgentFW(RandomGenerator* randomGenerator, Price* price,
                 double eta, double beta, double alpha_w, double alpha_n, double alpha_p, double alpha_0, double nu,
                 SwitchingStrategy switchingStrategy, const IndexStrategies& indexStrategy, double sigma) :
    Agent(randomGenerator, price),
    eta(eta),
    beta(beta),
    alpha_w(alpha_w),
    alpha_n(alpha_n),
    alpha_p(alpha_p),
    alpha_0(alpha_0),
    nu(nu),
    switchingStrategy(switchingStrategy),
    indexStrategy(indexStrategy),
    d(0),
    oldd(0),
    oldoldd(0),
    sigma(sigma),
    n(0.5),
    oldn(0.5),
    partner(nullptr)
{ }

void AgentFW::preStepUpdate() {
    oldPrice = price->getPrice();

    oldoldd = oldd;
    oldd = d;

    oldn = n;
}

void AgentFW::stepUpdate() {
    randomGenerator->getNormalRandomDouble(0, pow(sigma,2), &eps);

    d = calculateDemand();

    g = (exp(price->getPrice())-exp(oldPrice))*oldoldd;

    // calculated from w[t-1] and g[t] (!)
    w = eta*w + (1-eta)*g;

    // relative attractiveness
    oldAttractiveness = attractiveness;

    attractiveness = 0;
    attractiveness += calculateContributedAttractiveness();
    attractiveness += partner->calculateContributedAttractiveness();
    attractiveness += alpha_0;
}

void AgentFW::postStepUpdate(){
    tradingVolume = n*d;
}
