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
#ifndef PROJECT_PARAMETERSETDATAITEMCOLLECTOR_H
#define PROJECT_PARAMETERSETDATAITEMCOLLECTOR_H

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "../Util/Util.h"

#include "tinyxml2/tinyxml2.h"

using namespace std;
using namespace boost;

/** Definition/ParameterSet for the DataItemCollectors.
 */
class ParameterSetDataItemCollector{
public:
    optional<string> type; /**< Defines type of collector. For possibilities see the Wiki */
    optional<int> writeInterval; /**< At which timestep interval is this dataCollector written to disk. Default: none */
    optional<int> collectInterval; /**< At whicht timestep interval is this dataCollector collecting data. Default: 1 */
    optional<int> groupToTrack; /**< ID of the group (of Agents) that is tracked by this DataCollectorItem */
    optional<Util::DataItemCollectorMethod> method;

    bool validate();

    static vector<ParameterSetDataItemCollector> readFromXML(tinyxml2::XMLElement* dataItemCollectorClassesElement);
};


#endif //PROJECT_PARAMETERSETDATAITEMCOLLECTOR_H
