#include "ExcessDemandCalculatorFW.h"

ExcessDemandCalculatorFW::ExcessDemandCalculatorFW(std::vector<Agent*>* agents, ExcessDemand* excessDemand) :
    ExcessDemandCalculator(agents, excessDemand)
{

}

void ExcessDemandCalculatorFW::stepCalculate() {

    assert(agents != nullptr);
    assert(excessDemand != nullptr);

    double tempExcessDemand = 0;

    for (auto &agent : *agents) //iterate over all agents
        tempExcessDemand += agent->getTradingVolume();

    excessDemand->setExcessDemand(tempExcessDemand);
}
