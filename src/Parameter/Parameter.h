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

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>
#include <vector>
#include <boost/optional.hpp>

#include "ParameterSetAgent.h"
#include "ParameterSetExcessDemandCalculator.h"
#include "ParameterSetPriceCalculator.h"
#include "ParameterSetDataItemCollector.h"
#include "ParameterSetRNG.h"
#include "ParameterSetDividend.h"
#include <cstddef> //for std::size_t

using namespace std;


/** Class that defines a parameter object.
 * Contains the parameters that are needed for the simulation. Relies on boost::optionals since not all parameters have to be set for all kinds of simulations.
 */
class Parameter {
public:
	Parameter();
    Parameter(
              boost::optional<int>numSteps,
              boost::optional<double>deltaT,
              boost::optional<string>writerClass,
              boost::optional<string>outputname,
              ParameterSetExcessDemandCalculator parameterSetExcessDemandCalculator,
              boost::optional<double> startPrice,
              vector<ParameterSetAgent> parameterSetAgent,
              ParameterSetPriceCalculator parameterSetPriceCalculator,
              ParameterSetRNG parameterSetRNG,
              vector<ParameterSetDataItemCollector> parameterSetDataItemCollector,
			  ParameterSetDividend parameterSetDividend);
	virtual ~Parameter();

    bool validate();

    boost::optional<string> outputname; /**< Folder/Filename in which the output is written */


	//For ??
    vector<ParameterSetAgent> parameterSetAgent; /**< Vector containing all agent configurations */

    ParameterSetPriceCalculator parameterSetPriceCalculator; /**< PriceCalculator configuration */
    ParameterSetRNG parameterSetRNG; /**< Random Number Generator Configuration */
    vector<ParameterSetDataItemCollector> parameterSetDataItemCollector; /**< DataItemCollector Configuration */
	ParameterSetDividend parameterSetDividend; /**< Dividend Generator Configuration */


    ParameterSetExcessDemandCalculator parameterSetExcessDemandCalculator; /**< ExcessDemandCalculator configuration */

    boost::optional<string> writerClass; /**< Name of the Writer class */
    boost::optional<std::size_t> numSteps; /**< Numer of steps the simulation should run */
    boost::optional<double> deltaT; /**< Size of the timestep. Might influence some effects observed in simulations. */

	boost::optional<double> startPrice; /**< First price at the beginning of the simulation. */


	static vector<Parameter*> readFromFile(string filename);
};

#endif /* PARAMETER_H_ */
