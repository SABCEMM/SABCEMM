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
 * @file AgentEMB.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include <cstddef> //for std::size_t
#include "AgentEMB.h"

#include "../Algorithms/brent/brent.hpp"





/** Standard constructor of AgentEMB
 */
AgentEMB::AgentEMB():
        AgentEMB(nullptr, nullptr, 0, 0){


}


/** Constructor of AgentEMB. Requires a randomGenerator, a Price container,
 *  an ExcessDemand container and a GlobalNews container to work.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 */
AgentEMB::AgentEMB(RandomGenerator* newRandomGenerator, Price* newPrice):
        AgentEMB(newRandomGenerator, newPrice, 0, 0){


}


/** Constructor of AgentEMB. Requires a randomGenerator, a Price container, an ExcessDemand container
 *  and a GlobalNews container to work. Also sets the initial cash and stock of the agent.
 *  \param newRandomGenerator Pointer to the RandomGenerator
 *  \param newPrice Pointer to the Price container
 *  \param newCash Initial cash of the agent
 *  \param newStock Initial stock of the agent
 */
AgentEMB::AgentEMB(RandomGenerator* newRandomGenerator, Price* newPrice,
                   double newCash, double newStock):
        AgentEMB(newRandomGenerator, newPrice,
                 newCash,  newStock,
                 nullptr, 0,
                 0, 0,
                 0, 0,
                 0, 0)
{
}

AgentEMB::AgentEMB(RandomGenerator* newRandomGenerator, Price* newPrice,
                   double newCash, double newStock,
                   Dividend* newDividend, double newRiskTolerance,
                   std::size_t newMemorySpan, double newStdNoiseSigma,
                   double newInterestRate, double historyMean,
                   double historySigma, double initialGamma):
        Agent(newRandomGenerator, newPrice, newCash, newStock),
        riskTolerance(newRiskTolerance),
        memorySpan(newMemorySpan),
        stdNoiseSigma(newStdNoiseSigma),
        interestRate(newInterestRate),
        gamma(initialGamma),/// @todo do not hardcode - calculate from market clearance condition
        dividend(newDividend)
{
    assert(randomGenerator != nullptr);
    hypoCash = 0;

    oldGamma = gamma;
    iterGamma = gamma;

    relPriceHistory.clear();
    randomGenerator->getNormalRandomDouble(historyMean, historySigma, &relPriceHistory, memorySpan);

    oldPrice = price->getPrice(); //Init so sinnvoll?
}


/**
 */
void AgentEMB::updateCash() {

    assert(deltaT != nullptr);
    assert(price != nullptr);
    assert(dividend != nullptr);

    /** @todo
     * in an old commit, this has been changed to:
     *  cash =  deltaT->getDeltaT() * (
            (1-gamma) * (1+interestRate) * cash +
            oldGamma * cash * relPriceHistory.back());
     * Which version is correct?
     **/

    cash = cash + deltaT->getDeltaT() * (
            (1-oldGamma) * interestRate * cash
            + oldGamma * cash * relPriceHistory.back()
    );

}


/**
 */
void AgentEMB::updateTradingVolume(const double& newGamma, const double& newPrice, const double& newHypoCash){

    assert(price != nullptr);

    //

    tradingVolume = newGamma * newHypoCash / newPrice;


}


/**
 */
void AgentEMB::stepUpdate(){

    assert(price!=nullptr);
    assert(deltaT != nullptr);

    // Update gamma like in the last iteration of the bisection.
    // iterGamma is updated every call of the bisection. Therefore, after the bisection's
    // last iteration, it presents the optimal gamma for this iteration.
    oldGamma = gamma;
    gamma = iterGamma;

    double noisyGamma = 0;
    do{
        double noise = 0;
        randomGenerator->getNormalRandomDouble(0,stdNoiseSigma,&noise);
        noisyGamma = gamma + noise;
    }
    while(noisyGamma < 0.01 || noisyGamma > 0.99);

    gamma = noisyGamma;


    // return - next, the stock exchange updates price to be consistent with the noisy gammas.
    // the update continus in updateAgentAfterPriceUpdate().
}

void AgentEMB::preStepUpdate(){

}
void AgentEMB::postStepUpdate(){
    // price is now final for this iteration, we can update the history of returns.
    /// @todo oldPrice wird bspw. auch im excessDemandCalculator vorgehalten - eine globale History der Variablen
    /// sollte global (read-only) verfügbar sein - nicht der Performance wegen, sondern wegen der Korrektheit:
    /// Sowohl AgentEMB als auch der ED-Calculator müssen den alten Preis im preStep nachhalten, das ist fehleranfällig.
    relPriceHistory.push_back((price->getPrice() - oldPrice + deltaT->getDeltaT() * dividend->getDividend())/
                              (deltaT->getDeltaT() * oldPrice));
    //teile durch oldPrice nicht durch aktuellen price
    if(relPriceHistory.size() > memorySpan){
        relPriceHistory.erase(relPriceHistory.begin());
    }

    oldPrice = price->getPrice();

    updateCash();
}


/**
 */
void AgentEMB::updateBisection(const double& newIterPrice){


    hypoCash = cash + deltaT->getDeltaT() * ((1-gamma) * interestRate * cash
                                             + gamma * cash * (newIterPrice - price->getPrice() + deltaT->getDeltaT() * dividend->getDividend())/
                                               (deltaT->getDeltaT() * price->getPrice()));
    //durch price->getprice teilen nicht durch newITerPrice


    // most often, we can find appropriate gammas at the borders and rarely need to optimize.
    double dEULeft  = dExpectedUtility(0.01);
    double dEURight = dExpectedUtility(0.99);

    if(dEULeft < 0)
        iterGamma = 0.01;
    else if (dEULeft > 0)
    {
        if (dEURight > 0)
            iterGamma = 0.99;
        else if (dEURight < 0)
        {
            auto f = [this](double newGamma)->double { return this->dExpectedUtility(newGamma); };

            iterGamma = brent::zero(0.01, 0.99, 1e-2, f);


        }
    }

    updateTradingVolume(iterGamma, newIterPrice, hypoCash);


}



double AgentEMB::utility(double newCash) {
    double u = 0;
    if(Util::doubleNotEqual(riskTolerance,1.0)){
        u = pow(newCash,1-riskTolerance) / (1-riskTolerance);
    }
    else{
        u = log(newCash);
    }
    return u;
}

double AgentEMB::expectedUtility(double newGamma){
    double EU = 0;
    for (double i : relPriceHistory) {

        double res = (1-newGamma) * hypoCash * (1 + interestRate * deltaT->getDeltaT())
                     + newGamma * hypoCash * (1 + i * deltaT->getDeltaT());
        // relPriceHistory.at(i) weil die Reihenfolge bei der Summierunge unwichtig ist.

        EU += utility(res);
    }
    EU = EU / (double) relPriceHistory.size();
    return -EU; //returns -EU so that a minimizer can be used instead of a maximizer
}

double AgentEMB::dExpectedUtility(double gamma)
{
    double ret = 0;

    for(auto H_i : relPriceHistory)
        ret += (H_i-interestRate)* deltaT->getDeltaT()/(gamma*(H_i-interestRate)*deltaT->getDeltaT()+
                                                        1+interestRate*deltaT->getDeltaT());

    return ret;
}
