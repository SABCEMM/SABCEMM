//
// Created by max on 27.11.17.
//

#include "PriceCalculatorFW.h"

PriceCalculatorFW::PriceCalculatorFW(ExcessDemandCalculator* excessDemandCalculator, Price* price,
                                     ExcessDemand* excessDemand, double mu, AgentFW::SwitchingStrategy switchingStrategy):
    PriceCalculator(excessDemandCalculator, price, excessDemand), mu(mu), switchingStrategy(switchingStrategy)
{

}

void PriceCalculatorFW::stepCalculate() {
    // updated from variables at time t-1.
    excessDemandCalculator->stepCalculate();

    double new_price = 0;

    if(switchingStrategy == AgentFW::SwitchingStrategy::TPAC || switchingStrategy == AgentFW::SwitchingStrategy::TPACI) {
        throw "Current unpublished research...";
    }
    else{
            new_price = price->getPrice() + mu*excessDemand->getExcessDemand();
    }

    price->setPrice(new_price);
}
