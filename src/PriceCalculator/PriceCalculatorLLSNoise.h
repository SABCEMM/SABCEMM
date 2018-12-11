#ifndef PRICECALCULATORLLSNOISE_H
#define PRICECALCULATORLLSNOISE_H

#include "PriceCalculatorBisection.h"
#include "../RandomGenerator/RandomGenerator.h"


class PriceCalculatorLLSNoise : public PriceCalculatorBisection
{
private:
    double mu;
    double sigma;
    RandomGenerator& rng;
public:
    PriceCalculatorLLSNoise(
            ExcessDemandCalculator* newExcessDemandCalculator, Price* newPrice, ExcessDemand* newExcessDemand,
            bool adaptive, double low, double high, double epsilon, std::size_t maxIterations,
            double mu, double sigma,
            RandomGenerator& rng);
    virtual void postStepCalculate();
};


#endif // PRICECALCULATORLLSNOISE_H
