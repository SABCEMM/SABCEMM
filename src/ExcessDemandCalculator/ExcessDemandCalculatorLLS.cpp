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
 * @file ExcessDemandCalculatorLLS.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */


#include <cassert>
#include "ExcessDemandCalculatorLLS.h"
#include "../Agent/AgentLLS.h"
#include <cmath>





/** Constructor for the ExcessDemandCalculatorLLS.
 *
 *
 */
ExcessDemandCalculatorLLS::ExcessDemandCalculatorLLS(std::vector<Agent*>* newAgents, ExcessDemand* newExcessDemand,
                                                     Price* price, Dividend* dividend, std::string mode, double stocksPerAgent):
        ExcessDemandCalculator(newAgents, newExcessDemand),
        tempExcessDemand(0),
        stockPerAgent(stocksPerAgent),
        totalAmountOfStock(stocksPerAgent*(double)newAgents->size()),
        oldPrice(0),
        price(price),
        dividend(dividend)
{

    this->mode = stringToLLSEDMode.at(mode);
}


/** Standard destructor
 */
ExcessDemandCalculatorLLS::~ExcessDemandCalculatorLLS() = default;


/** Calculates the ExcessDemand
 * More information can be found in the paper cited in the class description.
 */
void ExcessDemandCalculatorLLS::stepCalculate() {

    static double numstep = 0;
    ++numstep;

	assert(agents != nullptr);
	assert(excessDemand != nullptr);

	tempExcessDemand = 0;

    for (auto &agent : *agents) //iterate over all agents
	{
		tempExcessDemand += agent->getTradingVolume();
    }

    switch(mode)
    {
        case LLSEDMode::original:
        {
            tempExcessDemand = tempExcessDemand / totalAmountOfStock - 1;
            break;
        }
        case LLSEDMode::substractInterest:
        {
            /// @todo hard coded, fix it! Introduces interdependencies between parameter sets, that is hard to solve.
            double interestRate = 0.04;
            tempExcessDemand = tempExcessDemand / totalAmountOfStock - 3.73*pow((1+interestRate), numstep);
            break;
        }
        case LLSEDMode::relaxExp:
        {
            const double relaxation = 4.6;
            tempExcessDemand = exp(-numstep/relaxation) * tempExcessDemand / totalAmountOfStock - 1;
            break;
        }
        case LLSEDMode::substractLastED:
        {
            // excessDemand->getExcessDemand() returns the current ED which is not yet updated.
            tempExcessDemand = tempExcessDemand / totalAmountOfStock - excessDemand->getExcessDemand();
            break;
        }
        case LLSEDMode::substractExpectedVolume:
        {
            double expectedVolume = 0;
            for (auto &agent : *agents) //iterate over all agents
                expectedVolume += agent->getCash();

            expectedVolume *= 0.5/price->getPrice();

            tempExcessDemand = tempExcessDemand /totalAmountOfStock -expectedVolume;
            break;
        }
        case LLSEDMode::normalize:
        {
            double sumOfWealth = 0;
            for (auto &agent : *agents)
                 sumOfWealth += agent->getCash();

            tempExcessDemand = tempExcessDemand / sumOfWealth - stockPerAgent;
            break;
        }
        default:
            throw "invalid LLSEDMode";
    }


    /** @todo Um das Paper zu treffen, muss hier tempExcessDemand *= P_h stehen.
     * Für ED ändert das nichts, aber sonst müssen wir mit der Abbruchbedingung aufpassen.
     */
	excessDemand->setExcessDemand(tempExcessDemand);
}

void ExcessDemandCalculatorLLS::preStepCalculate(){
    assert(price != nullptr);
    oldPrice = price->getPrice();
}
void ExcessDemandCalculatorLLS::postStepCalculate(){
    assert(excessDemand != nullptr);

    double term1 = 0;
    double term2 = 0;
    // should not change between iterations
    double numOfStocks = 0;

    for(Agent* a : *agents)
    {
        numOfStocks += a->getStock();
        // this cast will fail for anything that is not an AgentLLS, yielding NULL
        auto * aAsEMB = dynamic_cast<AgentLLS*>(a);
        if(aAsEMB == nullptr) {
            continue;
        }

        term1 += aAsEMB->getGamma()*aAsEMB->getOldGamma()*aAsEMB->getCash();
        term2 += aAsEMB->getGamma()*
                 ( aAsEMB->getCash() +
                   ( (1 - aAsEMB->getOldGamma())*aAsEMB->getInterestRate()*aAsEMB->getCash() ) +
                   ( aAsEMB->getOldGamma()*aAsEMB->getCash()*(-oldPrice+dividend->getDividend())/oldPrice) );

    }
    term1 = term1/numOfStocks/oldPrice;
    term2 = term2/numOfStocks;

    excessDemand->setLLSinfo(term1, term2);
}

void ExcessDemandCalculatorLLS::setTotalAmountOfStock(double newTotalAmountOfStock){
	totalAmountOfStock = newTotalAmountOfStock;
}

const std::map<std::string, ExcessDemandCalculatorLLS::LLSEDMode> ExcessDemandCalculatorLLS::stringToLLSEDMode =
        boost::assign::map_list_of("original", LLSEDMode::original)("substractinterest", LLSEDMode::substractInterest)
                ("relaxexp", LLSEDMode::relaxExp)("substractlasted", LLSEDMode::substractLastED)
                ("substractexpectedvolume", LLSEDMode::substractExpectedVolume)("normalize", LLSEDMode::normalize);
