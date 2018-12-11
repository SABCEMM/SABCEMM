#include "PriceCalculatorLLSNoise.h"

PriceCalculatorLLSNoise::PriceCalculatorLLSNoise(ExcessDemandCalculator* newExcessDemandCalculator,
                                                 Price* newPrice, ExcessDemand* newExcessDemand,
                                                 bool adaptive, double low, double high,
                                                 double epsilon, std::size_t maxIterations, double mu, double sigma, RandomGenerator& rng):
    PriceCalculatorBisection(newExcessDemandCalculator, newPrice, newExcessDemand, adaptive, low, high, epsilon, maxIterations),
    mu(mu),
    sigma(sigma),
    rng(rng)
{   }

void PriceCalculatorLLSNoise::postStepCalculate()
{
    // assuming the price has been found by bisection, add noise to it
    // P += (1+noise)*P
    double noise= 0;
    rng.getNormalRandomDouble(mu, sigma, &noise);
    price->setPrice(price->getPrice()*(1+noise));
}


