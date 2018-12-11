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

#include <string>
#include <vector>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "Simulation.h"
#include "Input/Input.h"

using namespace std;

/// simulations might be aborted to user or operating system interruption.
bool aborted;

void signalHandler(int s)
{
    cout << endl;
    cout << "signal '" << string(strerror(s)) << "' caught." << endl;
    cout << "Aborting the simulation sanely and flushing the results to disk..." <<  endl;
    aborted = true;
}

void registerSignalHandler()
{
#ifdef __linux__
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    sigaction(SIGTERM, &sigIntHandler, NULL);
    sigaction(SIGABRT, &sigIntHandler, NULL);
#endif
}

int main(int argc,char *argv[])
{

    registerSignalHandler();


	time_t start;
	time(&start);


    if(argc != 2)
    {
        cerr << "Expected exactly one argument." << endl;
        return -1;
    }
	string filename = argv[1];


	Input::fromFile inputs = Input::readFromFile(filename);

    bool success = Simulation::executeSimulations(inputs, aborted);

    //remove loaded xml from memory
    delete inputs.doc;
    inputs.doc = nullptr;

	time_t end;
	time(&end);

	if(success){
        std::cout << "All simulations finished successfully. Time: ";
	}else{
		std::cout << "There was an error during the simulations. Treat the results with care. Time: ";
	}

	std::cout << difftime(end, start) << " Seconds" << std::endl;



	return 0;
}
