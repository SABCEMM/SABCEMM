#ifndef PROJECT_EXCESSDEMANDCALCULATORFW_H
#define PROJECT_EXCESSDEMANDCALCULATORFW_H

#include "ExcessDemandCalculator.h"
#include "../Agent/Agent.h"
#include "../VariableContainer/ExcessDemand.h"

/// @ingroup Franke-Westerhoff
/// Excess demand calculator for the Franke-Westerhoff model.
class ExcessDemandCalculatorFW : public ExcessDemandCalculator{
public:
    ExcessDemandCalculatorFW(std::vector<Agent*>* agents, ExcessDemand* excessDemand);

    /// noop
    void preStepCalculate(){};
    /// Calculates excess demand as the sum of trading volumes
    void stepCalculate();
    /// noop
    void postStepCalculate(){};
};


#endif //PROJECT_EXCESSDEMANDCALCULATORFW_H
