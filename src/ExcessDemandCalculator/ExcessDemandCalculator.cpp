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
 * @file ExcessDemandCalculator.cpp
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#include "ExcessDemandCalculator.h"
#include "ExcessDemandCalculatorHarras.h"
#include "ExcessDemandCalculatorLLS.h"
#include "../VariableContainer/Dividend.h"
#include "ExcessDemandCalculatorFW.h"
#include "../Input/Input.h"


/** Standardconstructor
 */
ExcessDemandCalculator::ExcessDemandCalculator(): ExcessDemandCalculator(nullptr, nullptr){


}


/** Constructor for pure-virtual ExcessDemandCalculator.
 * Requires a pointer to a vector of agents and the ExcessDemandContainer.
 * \param newAgents Pointer to a vector of agents
 * \param newExcessDemand Pointer to the ExcessDemand container
 */
ExcessDemandCalculator::ExcessDemandCalculator(std::vector<Agent*>* newAgents, ExcessDemand* newExcessDemand){

	agents = newAgents;
	excessDemand = newExcessDemand;

}

ExcessDemandCalculator* ExcessDemandCalculator::factory(Input& input, std::vector<Agent*>* agents,
														ExcessDemand* excessDemand, Price* price, Dividend* dividend){

    Input& EDCinput = input["excessDemandCalculatorSettings"];

	ExcessDemandCalculator* excessDemandCalculator;
    if (EDCinput["excessDemandCalculatorClass"].getString() == "excessdemandcalculatorharras") {
		excessDemandCalculator = new ExcessDemandCalculatorHarras(agents,excessDemand);
	}
    else if (EDCinput["excessDemandCalculatorClass"].getString() == "excessdemandcalculatorlls") {
        double stocksPerAgent = 0;
        size_t agentCount = agents->size();
        for (auto &agent : *agents) {
            stocksPerAgent += agent->getStock();
        }
        stocksPerAgent /= double(agentCount);

        excessDemandCalculator = new ExcessDemandCalculatorLLS(agents, excessDemand, price, dividend,
                                                               EDCinput["excessDemandCalculatorClass"]("mode") ? EDCinput["excessDemandCalculatorClass"]["mode"].getString():"original",
                                                               stocksPerAgent);
	}
    else if (EDCinput["excessDemandCalculatorClass"].getString() == "excessdemandcalculatorfw"){
		excessDemandCalculator = new ExcessDemandCalculatorFW(agents, excessDemand);
	}

	else {
		throw("excessDemandCalculatorClass unknown!");
	}
	return excessDemandCalculator;
}

/** Destructor of the ExcessDemandCalculator
 */
ExcessDemandCalculator::~ExcessDemandCalculator() = default;


/** Setter method for the agents.
 */
void ExcessDemandCalculator::setAgents(std::vector<Agent*>* newAgents) {

	agents = newAgents;

}
