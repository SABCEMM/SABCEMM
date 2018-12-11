#ifndef PRICECALCULATORLLS1_H
#define PRICECALCULATORLLS1_H

#include "../Agent/Agent.h"
#include "../RandomGenerator/RandomGenerator.h"
#include "../VariableContainer/Price.h"
#include "../VariableContainer/ExcessDemand.h"
#include "../VariableContainer/DeltaT.h"
#include "../ExcessDemandCalculator/ExcessDemandCalculator.h"
#include "../PriceCalculator/PriceCalculator.h"

class PriceCalculatorLLS1 : public PriceCalculator
{
private:
    Price* price;
    DeltaT* deltaT;
    double marketDepth;
    ExcessDemand* ED;
    ExcessDemandCalculator* EDCalculator;
    std::vector<Agent*>* agents;
    RandomGenerator* rng;
    /// std. deviation of noise
    double sigma;
    /// mean of noise
    double mean;

    void stepCalculate();
    void preStepCalculate(){}
    void postStepCalculate(){}
public:
    PriceCalculatorLLS1(Price *price, DeltaT* deltaT, double marketDepth, ExcessDemand *ED, ExcessDemandCalculator* EDCalculator,
                        RandomGenerator* rng, double sigma, double mean,
                        std::vector<Agent*>* agents);
};

#endif // PRICECALCULATORLLS1_H
