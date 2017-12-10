//
// Created by max on 27.11.17.
//

#include "PriceCalculatorFW.h"

PriceCalculatorFW::PriceCalculatorFW(ExcessDemandCalculator* excessDemandCalculator, Price* price,
                                     ExcessDemand* excessDemand, double mu):
    PriceCalculator(excessDemandCalculator, price, excessDemand), mu(mu)
{

}

void PriceCalculatorFW::stepCalculate() {
    // updated from variables at time t-1.
    excessDemandCalculator->stepCalculate();

    price->setPrice( price->getPrice() + mu*excessDemand->getExcessDemand() );

}
