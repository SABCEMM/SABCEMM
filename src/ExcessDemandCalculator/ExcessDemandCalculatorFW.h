#ifndef PROJECT_EXCESSDEMANDCALCULATORFW_H
#define PROJECT_EXCESSDEMANDCALCULATORFW_H

#include "ExcessDemandCalculator.h"
#include "../Agent/Agent.h"
#include "../VariableContainer/ExcessDemand.h"

class ExcessDemandCalculatorFW : public ExcessDemandCalculator{
public:
    ExcessDemandCalculatorFW(std::vector<Agent*>* agents, ExcessDemand* excessDemand);

    void preStepCalculate(){};
    void stepCalculate();
    void postStepCalculate(){};
};


#endif //PROJECT_EXCESSDEMANDCALCULATORFW_H
