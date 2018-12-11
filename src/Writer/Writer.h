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

#ifndef WRITER_H_
#define WRITER_H_

#include <vector>
#include <string>

#include "../Input/Input.h"
#include "../Util/Util.h"
#include "../DataCollector/DataItemCollector.h"
#include "../QuantitiesOfInterest/QuantityOfInterest.h"
#include <cstddef> //for std::size_t

/** Pure-Virtual class that defines the default interface for a writer. A class that writes the results of a
 * simulation to disk.
 */
class Writer {
protected:
	std::string outputLocation;

public:
	Writer();
	virtual ~Writer();

	static Writer *factory(Input::fromFile input, int reps);

    /** Extracts and writes information from the parameter object which contains all input parameters.
     * \param newParameter Parameter object whose information is extracted.
     * \todo Warum Übergabe per Pointer?
     */
	virtual void saveInput(Input& input) = 0;
    /** Saves build-info.
     * @see buildinfo
     */
    virtual void saveBuildInfo() = 0;

	virtual void rngInformation(std::size_t &uniformGenerated, std::size_t &uniformUnused, std::size_t &normalGenerated,
                                    std::size_t &normalUnused, int &seed) = 0;

	virtual void saveTime(double time) = 0;

	virtual void addSimulation(std::string simulationIdentifier) = 0;
	virtual void addQoI(std::string method, QuantityOfInterest::Quantity quantity, int groupID,
						std::vector<std::vector<double>> *newVector, std::string &name_) = 0;
};

#endif /* WRITER_H_ */
