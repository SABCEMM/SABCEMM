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

#ifndef WRITERTXT_H_
#define WRITERTXT_H_

#include <vector>
#include <string>

#include "Writer.h"
#include <cstddef> //for std::size_t
#include <tinyxml2/tinyxml2.h>

using namespace tinyxml2;

/** Writes results to a txt file.
 * Implements Writer.
 */
class WriterTxt: public Writer {
protected:
    XMLDocument* doc;
    XMLNode* root;
    XMLNode* simulationElem;
    void parse(Input &input, XMLElement *elem);
    void XMLCheckResult(XMLError xmlError);
    int simulationNumber;
public:
	WriterTxt();
    explicit WriterTxt(const std::string& outputLocation);

	virtual ~WriterTxt();

	virtual void saveInput(Input& input);

	virtual void saveBuildInfo();

	virtual void rngInformation(std::size_t &uniformGenerated, std::size_t &uniformUnused, std::size_t &normalGenerated,
                                    std::size_t &normalUnused, int &seed);

	virtual void saveTime(double time);
	virtual void addSimulation(std::string simulationIdentifier);
	virtual void addQoI(std::string method, QuantityOfInterest::Quantity quantity, int groupID,
                        std::vector<std::vector<double>> *newVector, std::string &name_);
};

#endif /* WRITERTXT_H_ */
