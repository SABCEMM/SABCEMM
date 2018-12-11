#include "PriceCalculatorLLS1.h"

#include <iostream>
using namespace std;

PriceCalculatorLLS1::PriceCalculatorLLS1(Price *price, DeltaT* deltaT, double marketDepth, ExcessDemand *ED, ExcessDemandCalculator* EDCalculator, RandomGenerator* rng, double mean, double sigma, vector<Agent *> *agents):
    price(price),
    deltaT(deltaT),
    marketDepth(marketDepth),
    ED(ED),
    EDCalculator(EDCalculator),
    agents(agents),
    rng(rng),
    sigma(sigma),
    mean(mean)
{

}

void PriceCalculatorLLS1::stepCalculate()
{
    double noise;
    rng->getNormalRandomDouble(mean, sigma, &noise);

    // S_{k+1} = S_k + deltaT*marketDepth*ED_{k}
    price->setPrice(price->getPrice() + deltaT->getDeltaT()*marketDepth*ED->getExcessDemand()+noise);

    // In contrast to other price calculators, we defer the ED update.
    EDCalculator->stepCalculate();

    /// @todo demand update also nach dem Preisupdate, Reihenfolge ok?
    for(Agent* a : *agents)
        a->updateBisection(price->getPrice());
}
