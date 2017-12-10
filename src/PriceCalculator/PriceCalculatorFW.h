//
// Created by max on 27.11.17.
//

#ifndef PROJECT_PRICECALCULATORFW_H
#define PROJECT_PRICECALCULATORFW_H


#include "PriceCalculator.h"

class PriceCalculatorFW : public PriceCalculator{
public:
    PriceCalculatorFW(ExcessDemandCalculator* excessDemandCalculator, Price* price, ExcessDemand* excessDemand,
                      double mu);

    void preStepCalculate(){};
    void stepCalculate();
    void postStepCalculate(){};
private:
    const double mu;
};


#endif //PROJECT_PRICECALCULATORFW_H
