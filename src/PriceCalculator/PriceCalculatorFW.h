//
// Created by max on 27.11.17.
//

#ifndef PROJECT_PRICECALCULATORFW_H
#define PROJECT_PRICECALCULATORFW_H


#include "PriceCalculator.h"
#include "../Agent/AgentFW.h"

/// @ingroup Franke-Westerhoff
///
/// Calculates the price as in the Franke-Westerhoff model.
class PriceCalculatorFW : public PriceCalculator{
public:
    PriceCalculatorFW(ExcessDemandCalculator* excessDemandCalculator, Price* price, ExcessDemand* excessDemand,
                      double mu, AgentFW::SwitchingStrategy switchingStrategy);

    /// noop
    void preStepCalculate(){};
    /// Updates excess demand (time t-1), then calculates the price (time t).
    /// @todo 1/2*(2*demand_c+2*demand_f)?
    void stepCalculate();
    /// noop
    void postStepCalculate(){};
protected:
    const double mu;
    AgentFW::SwitchingStrategy switchingStrategy;

};


#endif //PROJECT_PRICECALCULATORFW_H
