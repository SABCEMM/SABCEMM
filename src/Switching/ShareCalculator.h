#ifndef SHARECALCULATOR_H
#define SHARECALCULATOR_H

#include <map>
#include <vector>
#include <cmath>
#include <cstddef>
#include <cstdlib>

#include <boost/assign.hpp>

#include "Switchable.h"
#include "../VariableContainer/Price.h"



class ShareCalculator
{
public:
    /// Called as soon as the price is final.
    /// @todo does that make sense? Or should the shares be determined at the beginning?
    void updateShares();
    // alpha_n = 1.79, alpha_0=0.8 oder alpha_0=-0.327
    ShareCalculator(std::vector<Switchable*> switchables, Price& price, std::string strategy, double alpha_n, double alpha_0);

    enum ShareCalculatorStrategy
    {
        pdivpf = 0,
        ratio,
        ratiowithfunction
    };

    static const std::map<std::string, ShareCalculatorStrategy> stringToShareCalculatorStrategy;
private:
    ShareCalculatorStrategy strategy;

    double alpha_n;
    double alpha_0;

    std::vector<Switchable*> switchables;

    const double beta = 1;

    Price& price;

    std::vector<double> attractiveness;

    double ratePrices(double price, double fundamentalPrice);

};

#endif // SHARECALCULATOR_H
