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
 * @file Agent.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "Agent.h"
#include "AgentCross.h"
#include "AgentHarras.h"
#include "AgentCrossWealth.h"
#include "AgentLLS.h"
#include "AgentFW.h"
#include "AgentFWFundamentalist.h"
#include "AgentFWChartist.h"

#include "../RandomGenerator/RandomGenerator.h"
#include "../Switching/LLSGroup.h"
#include "../VariableContainer/DeltaT.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/GlobalNews.h"
#include "../VariableContainer/Dividend.h"

#include "../Input/Input.h"
#include "../NeighbourhoodGenerator/LatticeNeighbourhoodGenerator.h"


#include <string>
#include <cassert>
#include <vector>

#include <iostream>
using namespace std;


/** Standard constructor for the Agent.
 */
Agent::Agent() : Agent(nullptr, nullptr, 0, 0) {


}

/** Constructor for the pure-virtual Agent.
 * Requires a RandomGenerator and the Price Container.
 * \param newRandomGenerator Pointer on the RandomGenerator
 * \param newPrice Pointer to the Price container
 */
Agent::Agent(RandomGenerator *newRandomGenerator, Price *newPrice) : Agent(newRandomGenerator, newPrice, 0, 0) {


}

/** Extended constructor for the pure-virtual Agent.
 * Requires a RandomGenerator  and the Price Container.. Initializes the cash and the stock of
 * an agent to user-defined values.
 * \param newRandomGenerator Pointer on the RandomGenerator
 * \param newPrice Pointer to the Price container
 * \param newCash Amount of cash
 * \param newStock Amount of stock
 */
Agent::Agent(RandomGenerator *newRandomGenerator, Price *newPrice, double newCash, double newStock) {

    cash = newCash;
    stock = newStock;
    decision = 0;
    tradingVolume = 0;
    randomGenerator = newRandomGenerator;
    price = newPrice;
    deltaT = nullptr;

}


std::vector<Agent*> *Agent::factory(Input &input, RandomGenerator *randomNumberPool, Price *price,
                                     ExcessDemand *excessDemand, GlobalNews *globalNews, Dividend *dividend,
                                     DeltaT *deltaT, std::vector<Switchable *> &switchableGroups) {
    std::vector<Agent*> *agents = new std::vector<Agent *>;

    std::vector<Agent*> tempHarrasAgents;

    //Create Agents by Iterating over AgentClasses

    if (!input("agents")) {
        input["agents"]; // Abort if no agents are configured
    }


    for (auto& agentInput : input["agents"].getChildren()) {
        tempHarrasAgents.clear();

        // Make Code more readable
        string type = agentInput("type") ? agentInput["type"].getString() : agentInput.getName();

        /// @todo LLS agents have a different mechanism for count, but this should be unified.
        if (type == "agentlls") {
            const size_t embCount = agentInput("AgentEMB") ? agentInput["AgentEMB"]["count"].getSizeT() : 0;
            const size_t riiCount = agentInput("AgentRII") ? agentInput["AgentRII"]["count"].getSizeT() : 0;
            const size_t totalCount = embCount + riiCount;
            LLSGroup *group = new LLSGroup(*randomNumberPool,
                                           *dividend,
                                           agentInput("AgentRII") && agentInput["AgentRII"]("k")
                                           ? agentInput["AgentRII"]["k"].getDouble() : 0,
                                           *price,
                                           agentInput("alpha_p1") ? agentInput["alpha_p1"].getDouble() : 0,
                                           agentInput("alpha_p2") ? agentInput["alpha_p2"].getDouble() : 0);

            for (size_t i = 0; i < totalCount; ++i) {
                AgentLLS::MemorySpanMode memSpanMode = AgentLLS::MemorySpanMode::fixed;
                if(agentInput("AgentEMB"))
                {
                    if(agentInput["AgentEMB"]("memorySpan"))
                    {
                        if(agentInput["AgentEMB"]("memorySpanMu") || agentInput["AgentEMB"]("memorySpanSigma") )
                        {
                            cerr << "memorySpan and (memorySpanMu, memorySpanSigma) are mutually exclusive." << endl;
                            continue;
                        }
                        memSpanMode = AgentLLS::MemorySpanMode::fixed;
                    }
                    else if(agentInput["AgentEMB"]("memorySpanMu") || agentInput["AgentEMB"]("memorySpanSigma"))
                    {
                         if(!agentInput["AgentEMB"]("memorySpanMu") || !agentInput["AgentEMB"]("memorySpanSigma") )
                         {
                             cerr << "memorySpanMu or memorySpanSigma not specified." << endl;
                             continue;
                         }
                         memSpanMode = AgentLLS::MemorySpanMode::random;
                    }
                    else
                    {
                        cerr << "AgentEMB lacks settings memory span settings." << endl;
                        continue;
                    }
                }

                auto tempAgent = new AgentLLS(randomNumberPool,
                                               price,
                                               agentInput["cash"].getDouble(),
                                               agentInput["stock"].getDouble(),
                                               dividend,
                                               agentInput["riskTolerance"].getDouble(),
                                               agentInput["stdNoiseSigma"].getDouble(),
                                               input["dividendSettings"]["interestRate"].getDouble(),
                                               agentInput["initialGamma"].getDouble(),
                        // we expect validation to be done in the parameter object
                                               agentInput("AgentRII") && agentInput["AgentRII"]("k") ? agentInput["AgentRII"]["k"].getDouble() : 0,
                                               agentInput("AgentRII") && agentInput["AgentRII"]("utility")? agentInput["AgentRII"]["utility"].getString() : "original",
                                               agentInput("AgentRII") && agentInput["AgentRII"]("group") ? agentInput["AgentRII"]["group"].getIntSet() : set<int>(),
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("utility") ? agentInput["AgentEMB"]["utility"].getString() : "original",
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("group") ? agentInput["AgentEMB"]["group"].getIntSet() : set<int>(),
                                               agentInput("AgentEMB") ? agentInput["AgentEMB"]("historyMean") : 0,
                                               agentInput("AgentEMB") ? agentInput["AgentEMB"]("historySigma") : 0,
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("gammaMin") ? agentInput["AgentEMB"]["gammaMin"].getDouble() : 0.01,
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("gammaMax") ? agentInput["AgentEMB"]["gammaMax"].getDouble() : 0.99, 
                                               memSpanMode,
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("memorySpan") ? agentInput["AgentEMB"]["memorySpan"].getSizeT() : 0,
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("memorySpanMu") ? agentInput["AgentEMB"]["memorySpanMu"].getDouble() : 0,
                                               agentInput("AgentEMB") && agentInput["AgentEMB"]("memorySpanSigma") ? agentInput["AgentEMB"]["memorySpanSigma"].getDouble() : 0
                );

                if (agentInput("groups")) {
                    tempAgent->setGroups(agentInput["groups"].getIntVector());
                }

                agents->push_back(static_cast<Agent*>(tempAgent));

                group->addToGroup(tempAgent);
            }

            for (size_t i = 0; i < embCount; ++i)
                static_cast<AgentLLS*>(agents->at(i))->setStrategy(AgentLLS::LLSStrategy::EMB);
            for (size_t i = embCount; i < totalCount; ++i)
                static_cast<AgentLLS*>(agents->at(i))->setStrategy(AgentLLS::LLSStrategy::RII);
            switchableGroups.push_back(group);
        } else {
            for (std::size_t j = 0; j < agentInput["count"].getSizeT(); j++) {
                if (type == "agentharras") {
                    auto tempAgent = new AgentHarras(randomNumberPool, price, excessDemand, globalNews,
                                                      agentInput["cash"].getDouble(), agentInput["stock"].getDouble(),
                                                      agentInput["C1"].getDouble(), agentInput["C2"].getDouble(),
                                                      agentInput["C3"].getDouble(), agentInput["threshold"].getDouble(),
                                                      agentInput["g"].getDouble(), agentInput["alpha"].getDouble(),
                                                      input["priceCalculatorSettings"]["marketDepth"].getDouble());

                    if (agentInput("groups")) {
                        tempAgent->setGroups(agentInput["groups"].getIntVector());
                    }
                    tempHarrasAgents.push_back(static_cast<Agent *>(tempAgent));
                } else if (type == "agentcross") {
                    auto tempAgent = new AgentCross(randomNumberPool, price, excessDemand,
                                                     agentInput["cash"].getDouble(), agentInput["stock"].getDouble(),
                                                     deltaT,
                                                     agentInput["b1"].getDouble(), agentInput["b2"].getDouble(),
                                                     agentInput["A1"].getDouble(), agentInput["A2"].getDouble());
                    if (agentInput("groups")) {
                        tempAgent->setGroups(agentInput["groups"].getIntVector());
                    }
                    agents->push_back(static_cast<Agent *>(tempAgent));
                } else if (type == "agentcrosswealth") {
                    auto tempAgent = new AgentCrossWealth(randomNumberPool,
                                                           price,
                                                           excessDemand,
                                                           agentInput["cash"].getDouble(),
                                                           agentInput["stock"].getDouble(),
                                                           deltaT,
                                                           agentInput["b1"].getDouble(), agentInput["b2"].getDouble(),
                                                           agentInput["A1"].getDouble(), agentInput["A2"].getDouble(),
                                                           agentInput["gamma"].getDouble(),
                                                           input["dividendSettings"]["interestRate"].getDouble());

                    if (agentInput("groups")) {
                        tempAgent->setGroups(agentInput["groups"].getIntVector());
                    }
                    agents->push_back(static_cast<Agent *>(tempAgent));
                } else if (type == "agentfw") {


                    // this is one of the rare cases where we actually rely on default values for null-optionals.
                    // The reasoning here is: We assume that the Parameter object validated that for the chosen index
                    // strategy, the correct variables are given. Otherwise, the program would have been aborted.
                    // For certain index strategies, not all coefficients are used - they are therefore not specified in the
                    // input file. Still, AgentFW expects these variables to be given. As they are not used, 0 is chosen
                    // but any value would be fine.
                    // We should NOT use this method in the Parameter object (therefore initializing all alphas with 0 here)
                    // as we cannot warn the user about unspecified variables in the input file then.
                    // The same reasoning applies to nu (which is only needed for TPA) and beta
                    // (which is only needed for DCA)
                    auto a1 = new AgentFWFundamentalist(
                            randomNumberPool,
                            price,
                            agentInput("eta") ? agentInput["eta"].getDouble() : 0,
                            agentInput("beta") ? agentInput["beta"].getDouble() : 0,
                            agentInput("alpha_w") ? agentInput["alpha_w"].getDouble() : 0,
                            agentInput("alpha_n") ? agentInput["alpha_n"].getDouble() : 0,
                            agentInput("alpha_p") ? agentInput["alpha_p"].getDouble() : 0,
                            agentInput("alpha_0") ? agentInput["alpha_0"].getDouble() : 0,
                            agentInput("nu") ? agentInput["nu"].getDouble() : 0,
                            AgentFW::stringToSwitchingStrategy.at(agentInput["switchingStrategy"].getString()),
                            agentInput["indexStrategy"].getString(),
                            agentInput["AgentFundamentalist"]["sigma"].getDouble(),
                            agentInput["AgentFundamentalist"]["phi"].getDouble(),
                            agentInput["AgentFundamentalist"]["fundamentalPrice"].getDouble()
                    );
                    auto a2 = new AgentFWChartist(
                            randomNumberPool,
                            price,
                            agentInput["eta"].getDouble(),
                            agentInput("beta") ? agentInput["beta"].getDouble() : 0,
                            agentInput("alpha_w") ? agentInput["alpha_w"].getDouble() : 0,
                            agentInput("alpha_n") ? agentInput["alpha_n"].getDouble() : 0,
                            agentInput("alpha_p") ? agentInput["alpha_p"].getDouble() : 0,
                            agentInput("alpha_0") ? agentInput["alpha_0"].getDouble() : 0,
                            agentInput("nu") ? agentInput["nu"].getDouble() : 0,
                            AgentFW::stringToSwitchingStrategy.at(agentInput["switchingStrategy"].getString()),
                            agentInput["indexStrategy"].getString(),
                            agentInput["AgentChartist"]["sigma"].getDouble(),
                            agentInput["AgentChartist"]["chi"].getDouble()
                    );

                    if (agentInput("groups")) {
                        a1->setGroups(agentInput["groups"].getIntVector());
                        a2->setGroups(agentInput["groups"].getIntVector());
                    }
                    a1->setPartner(a2);
                    a2->setPartner(a1);

                    //adjust shares if offset necessary
                    a1->setShare(agentInput("delta_n") ? agentInput["delta_n"].getDouble()+0.5 : 0.5);

                    agents->push_back(dynamic_cast<Agent *>(a1));
                    agents->push_back(dynamic_cast<Agent *>(a2));
                }
                else {
                    cerr << "agent class " << type << "unknown. Skipping.";
                }
            }
        }

        //If HarrasAgents are used create NeighbourhoodGenerator and create Neighbourhood

        if (type == "agentharras") {
            NeighbourhoodGenerator *neighbourhoodGenerator; /**< Pointer to a NeighbourhoodGenerator */
            if (agentInput["neighbourhoodGeneratorClass"].getString()
                == "latticeneighbourhoodgenerator") {
                neighbourhoodGenerator = new LatticeNeighbourhoodGenerator(&tempHarrasAgents);
            } else {
                throw ("neighbourhoodGeneratorClass unknown!");
            }

            neighbourhoodGenerator->generateNeighbourhood();

            agents->insert(agents->end(), tempHarrasAgents.begin(), tempHarrasAgents.end());
            delete neighbourhoodGenerator;
        }
    }

    for (auto agent : *agents) {
        agent->setDeltaT(deltaT);
    }

    return agents;
}

/** Virtual destructor of the Agent.
 */
Agent::~Agent() = default;

/** Getter method for the agent's current cash.
 * \return Agent's current cash
 */
double Agent::getCash() const {


    return cash;
}

/** Setter method for the agent's current cash.
 * \param newCash New amount of cash for the agent
 */
void Agent::setCash(double newCash) {

    cash = newCash;

}

/** Getter method for agent's current decision.
 * \return Agent's current decision
 */
int Agent::getDecision() const {


    return decision;
}

/** Setter method for the randomGenerator.
 */
void Agent::setRandomGenerator(RandomGenerator *newRandomGenerator) {

    randomGenerator = newRandomGenerator;

}

/** Setter method for the Price container
 * \param newPrice Pointer to the new Price container
 */
void Agent::setPrice(Price *newPrice) {

    price = newPrice;

}

/** Getter method for the agent's current stock.
 */
double Agent::getStock() const {


    return stock;
}

/** Setter methods for the agents stock
 * \param newStock
 */
void Agent::setStock(double newStock) {
    stock = newStock;
}

/** Getter method for the agent's current trading volume.
 */
double Agent::getTradingVolume() const{
    return tradingVolume;
}


void Agent::setDeltaT(DeltaT *newDeltaT) {
    deltaT = newDeltaT;
}
