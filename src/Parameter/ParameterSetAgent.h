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

#ifndef PROJECT_PARAMETERSETAGENT_H
#define PROJECT_PARAMETERSETAGENT_H

#include <string>
#include <vector>
#include <boost/optional.hpp>

#include "tinyxml2/tinyxml2.h"
#include <cstddef> //for std::size_t
#include <bitset>

using namespace std;

/** Parameters for the Agent from the paper "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette.
 * @todo count is a required parameter for all Agents but AgentFW.
 */
class ParameterSetAgent{
public:
    //General Parameters
    boost::optional<string> type; /**<  */
    boost::optional<std::size_t> count; /**<  */
    boost::optional<double> cash; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<double> stock; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<vector<int>> groups; /**< Classify Agents into different groups. A DataItemCollector can track one
 * group, if they match. */

    //Agent Harras
    boost::optional<double> C1; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<double> C2; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<double> C3; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<double> threshold; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges
 * Harras and Didier Sornette for details. */
    boost::optional<double> g; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<double> alpha; /**< See "How to grow a bubble: A model of myopic adapting agents" by Georges Harras
 * and Didier Sornette for details. */
    boost::optional<string> neighbourhoodGeneratorClass; /**< Right now only lattice neighbourhood generator possible.*/

    // @TODO : Add paper references for Doku
    //Agent Cross
    boost::optional<double> A1; /**< See "A Threshold Model of Investor Psychology (2005)"
 * by Rod Cross et. al for details. */
    boost::optional<double> A2; /**< See "A Threshold Model of Investor Psychology (2005)"
 * by Rod Cross et. al for details. */
    boost::optional<double> b1; /**< See "A Threshold Model of Investor Psychology (2005)"
 * by Rod Cross et. al for details. */
    boost::optional<double> b2; /**< See "A Threshold Model of Investor Psychology (2005)"
 * by Rod Cross et. al for details. */
    boost::optional<double> gamma; /**< See "A Threshold Model of Investor Psychology (2005)"
 * by Rod Cross et. al for details. */

    //General LLS
    boost::optional<double> stdNoiseSigma; /**<  */
    boost::optional<double> riskTolerance; /**<  */
    boost::optional<double> historyMean;
    boost::optional<double> historySigma;
    boost::optional<double> initialGamma;

    //Agent EMB
    boost::optional<int> memorySpan; /**<  */

    //Agent RII
    boost::optional<double> k; /**<  */

    // Agent Franke-Westerhoff
    boost::optional<double> eta;
    boost::optional<string> switchingStrategy;
    boost::optional<double> beta;
    boost::optional<std::bitset<4> > indexStrategy;
    boost::optional<double> alpha_w;
    boost::optional<double> alpha_n;
    boost::optional<double> alpha_p;
    boost::optional<double> alpha_0;
    boost::optional<double> nu;

    // chartist
    boost::optional<double> sigma_c;
    boost::optional<double> chi;

    // fundamentalist
    boost::optional<double> sigma_f;
    boost::optional<double> phi;
    boost::optional<double> fundamentalPrice;

    bool validate();

    static vector<ParameterSetAgent> readFromXML(tinyxml2::XMLElement* agent);
};

#endif //PROJECT_PARAMETERSETAGENT_H
