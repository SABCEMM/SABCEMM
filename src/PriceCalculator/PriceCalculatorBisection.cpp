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

#include <cassert>
#include <complex>
#include "../Util/Util.h"
#include "PriceCalculatorBisection.h"

#include <iostream>
using namespace std;
//Begin EXPORT ED
//#include "../Writer/WriterTxt.h"
//#include <vector>
//END






template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

/** Standarddestructor
 */
PriceCalculatorBisection::~PriceCalculatorBisection() = default;


/**
 */
void PriceCalculatorBisection::stepCalculate() {

	assert(excessDemandCalculator != nullptr);
	assert(excessDemand != nullptr);
	assert(price != nullptr);


    double a,b;

    if(adaptive)
    {
        a = low*price->getPrice();
        b = high*price->getPrice();
    }
    else
    {
        a = low;
        b = high;
    }

    double m = (a+b)/2;

    // That resembles the original behavior - is that correct for the intial iteration?
    if(Util::doubleEqual(getExcessDemandAtPrice(a) ,0.0))
    {

        price->setPrice(a);
        return;
    }
    else if(Util::doubleEqual(getExcessDemandAtPrice(b), 0.0))
    {

        price->setPrice(b);
        return;
    }

    bool healthy = sign(getExcessDemandAtPrice(a)) != sign(getExcessDemandAtPrice(b));

    // 0.000998254


    double minExcessDemand = numeric_limits<double>::max();
    double minimizingPrice;


    if(!healthy)
    {

        const int MAX_PRICE = (int)b*10;

        // assuming that f is a falling function.
        for(int new_b = (int)b; b < MAX_PRICE ; new_b*=2)
        {
            a = b;
            b = new_b;

            const double excessAtNewB = getExcessDemandAtPrice(new_b);

            if(sign(excessAtNewB) != sign(getExcessDemandAtPrice(a)))
            {

                healthy = true;

                break;
            }

            if(abs(excessAtNewB) < minExcessDemand)
            {
                minExcessDemand = abs(excessAtNewB);
                minimizingPrice = new_b;

                // we found a satisfying minimizer (via linear search, not bisection.)
                // This minimizer will be used later in this function.
                if(minExcessDemand < epsilon)
                    break;
            }
        }
    }
    if(healthy)
    {
        size_t i = 0;
        for(m = (a+b)/2; abs(getExcessDemandAtPrice(m)) > epsilon; m = (a+b)/2)
        {
            double demandAtA = getExcessDemandAtPrice(a);
            if(sign(demandAtA) == sign(getExcessDemandAtPrice(b)))
            {

                return;
            }

            ++i;

            /**





            **/

            if(sign(demandAtA) == sign(getExcessDemandAtPrice(m)))
                a = m;
            else
                b = m;

            if(i > maxIterations)
            {
                cerr << "bisection did not converge" << endl;
                throw NoConvergenceException();
            }
        }



    }

    if(!healthy)
    {
        m = minimizingPrice;
        if (minExcessDemand > epsilon){
            throw BadDomainError();
        }
    }


    price->setPrice(m);


}

void PriceCalculatorBisection::preStepCalculate(){

}
void PriceCalculatorBisection::postStepCalculate(){

}


/** Constructor for the PriceCalculatorHarras. For Documentation see parent class PriceCalculator.
 */
PriceCalculatorBisection::PriceCalculatorBisection(ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice,
                                                   ExcessDemand* newExcessDemand, bool adaptive, double low, double high, double epsilon, size_t maxIterations):
    PriceCalculator(newExcessDemandCalculator, newPrice, newExcessDemand),
    epsilon(epsilon),
    maxIterations(maxIterations),
    adaptive(adaptive),
    low(low),
    high(high)
{
    if(adaptive)
        assert(low < high);
    else
        assert(low+epsilon<high-epsilon);
	agents = nullptr;
}

void PriceCalculatorBisection::setAgents(std::vector<Agent*>* newAgents){
    assert(newAgents != nullptr);
    agents = newAgents;
}

double PriceCalculatorBisection::getExcessDemandAtPrice(double iterPrice){

	assert(agents != nullptr);

	for (auto &agent : *agents) {
        agent->updateBisection(iterPrice);
	}

	excessDemandCalculator->stepCalculate();

	return excessDemand->getExcessDemand();
}

/**double PriceCalculatorBisection::getExcessDemandAtPrice(double iterPrice){
	return iterPrice-3.3;
}**/
