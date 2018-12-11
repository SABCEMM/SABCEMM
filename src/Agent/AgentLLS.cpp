#include "AgentLLS.h"
#include <cassert>
#include <iostream>
#include "../Algorithms/brent/brent.hpp"
#include "../Util/Util.h"

AgentLLS::AgentLLS(RandomGenerator *newRandomGenerator, Price *newPrice,
                   double newCash, double newStock,
                   Dividend *newDividend, double newRiskTolerance,
                   double newStdNoiseSigma,
                   double newInterestRate,
                   double initialGamma,
                   double k,
                   std::string riiUtility,
                   const std::set<int> riiGroup,
                   std::string embUtility,
                   const std::set<int> embGroup,
                   double historyMean,
                   double historySigma,
                   double gammaMin,
                   double gammaMax,
                   MemorySpanMode memorySpanMode,
                   size_t memorySpan,
                   double memorySpanMu,
                   double memorySpanSigma)
        : Agent(newRandomGenerator, newPrice, newCash, newStock),
          riskTolerance(newRiskTolerance),
          stdNoiseSigma(newStdNoiseSigma),
          interestRate(newInterestRate),
          gamma(initialGamma),/// @todo do not hardcode - calculate from market clearance condition
          iterGamma(initialGamma),
          oldGamma(initialGamma),
          k(k),
          riiGroup(riiGroup),
          embGroup(embGroup),
          dividend(newDividend),
          gammaMin(gammaMin),
          gammaMax(gammaMax) {
    if (memorySpanMode == MemorySpanMode::fixed)
        this->memorySpan = memorySpan;
    else {
        int _memorySpan;
        randomGenerator->getNormalRandomInt(memorySpanMu, memorySpanSigma, &_memorySpan);
        this->memorySpan = _memorySpan;
    }

    this->riiUtility = AgentLLS::stringToRIIUtility.at(riiUtility);
    this->embUtility = AgentLLS::stringToEMBUtility.at(embUtility);

    setStrategy(LLSStrategy::EMB);

    assert(randomGenerator != nullptr);
    hypoCash = 0;

    oldGamma = gamma;
    iterGamma = gamma;

    oldPrice = price->getPrice(); //Init so sinnvoll?

    relPriceHistory.clear();
    randomGenerator->getNormalRandomDouble(historyMean, historySigma, &relPriceHistory, memorySpan);
}

void AgentLLS::updateTradingVolume(double newGamma, double newPrice, double newHypoCash) {
    assert(price != nullptr);

    tradingVolume = newGamma * newHypoCash / newPrice;
}

void AgentLLS::postStepUpdate() {
    // price is now final for this iteration, we can update the history of returns.
    /// @todo oldPrice wird bspw. auch im excessDemandCalculator vorgehalten - eine globale History der Variablen
    /// sollte global (read-only) verfügbar sein - nicht der Performance wegen, sondern wegen der Korrektheit:
    /// Sowohl AgentEMB als auch der ED-Calculator müssen den alten Preis im preStep nachhalten, das ist fehleranfällig.
    /// @todo Ringpufferklasse verwenden
    relPriceHistory.push_back((price->getPrice() - oldPrice + deltaT->getDeltaT() * dividend->getDividend()) /
                              (deltaT->getDeltaT() * oldPrice));
    //teile durch oldPrice nicht durch aktuellen price
    if (relPriceHistory.size() > memorySpan) {
        relPriceHistory.erase(relPriceHistory.begin());
    }

    updateCash();
    oldPrice = price->getPrice();
}

void AgentLLS::updateCash() {

    assert(deltaT != nullptr);
    assert(price != nullptr);
    assert(dividend != nullptr);

    /** @todo
     * in an old commit, this has been changed to:
     *  cash =  deltaT->getDeltaT() * (
            (1-gamma) * (1+interestRate) * cash +
            oldGamma * cash * relPriceHistory.back());
     * Which version is correct?
     **/

    const double currentReturn = (price->getPrice() - oldPrice + deltaT->getDeltaT() * dividend->getDividend()) /
                                 (deltaT->getDeltaT() * oldPrice);

    cash = cash + deltaT->getDeltaT() * (
            (1 - oldGamma) * interestRate * cash
            + oldGamma * cash * currentReturn
    );
}

void AgentLLS::stepUpdate() {
    assert(price != nullptr);
    assert(deltaT != nullptr);

    // Update gamma like in the last iteration of the bisection.
    // iterGamma is updated every call of the bisection. Therefore, after the bisection's
    // last iteration, it presents the optimal gamma for this iteration.
    oldGamma = gamma;
    gamma = iterGamma;

    double noisyGamma = 0;
    if (Util::doubleEqual(stdNoiseSigma, 0))
        noisyGamma = gamma;
    else {
        do {
            double noise = 0;
            randomGenerator->getNormalRandomDouble(0, stdNoiseSigma, &noise);
            noisyGamma = gamma + noise;
        } while (noisyGamma < gammaMin || noisyGamma > gammaMax);
    }

    gamma = noisyGamma;


    // return - next, the stock exchange updates price to be consistent with the noisy gammas.
    // the update continues in postStepUpdate().
}

double AgentLLS::EMBexpectedUtility(double newGamma) {
    double EU = 0;

    switch (embUtility) {
        case EMBUtility::original: {
            for (double H_i : relPriceHistory) {

                double res = (1 - newGamma) * hypoCash * (1 + interestRate * deltaT->getDeltaT())
                             + newGamma * hypoCash * (1 + H_i * deltaT->getDeltaT());
                // relPriceHistory.at(i) weil die Reihenfolge bei der Summierunge unwichtig ist.

                EU += EMButility(res);
            }
            EU = EU / (double) relPriceHistory.size();

            break;
        }
        case EMBUtility::book: {
            for (double H_i : relPriceHistory) {
                EU += pow(((1 - newGamma) * (1 + interestRate * deltaT->getDeltaT()) + newGamma * H_i),
                          1 - riskTolerance);
            }

            EU *= pow(hypoCash, 1 - riskTolerance) / (1 - riskTolerance) / (double) relPriceHistory.size();
        }
    }
    return -EU; //returns -EU so that a minimizer can be used instead of a maximizer
}

double AgentLLS::EMButility(double newCash) {
    double u = 0;
    if (Util::doubleNotEqual(riskTolerance, 1.0)) {
        u = pow(newCash, 1 - riskTolerance) / (1 - riskTolerance);
    } else {
        u = log(newCash);
    }
    return u;
}

double AgentLLS::EMBdExpectedUtility(double gamma) {
    double ret = 0;

    for (auto H_i : relPriceHistory)
        ret += (H_i - interestRate) * deltaT->getDeltaT() / (gamma * (H_i - interestRate) * deltaT->getDeltaT() +
                                                             1 + interestRate * deltaT->getDeltaT());

    return ret;
}

double AgentLLS::RIIexpectedUtility(double gamma, double P_h, double W_h) {
    assert(Util::doubleNotEqual(riskTolerance, 1.0));
    double EU = 0;

    if (riiUtility == RIIUtility::original) {
        // definitions as in the Jupyter prototype.
        const double Apre = 1 / ((1 - riskTolerance) * (2 - riskTolerance) * (dividend->getZ2() - dividend->getZ1())) *
                            ((k - dividend->getExpectedIncrease()) / (k + 1)) * P_h / dividend->getDividend();
        const double A = pow(W_h, 1 - riskTolerance) * Apre;
        const double B = 1 + interestRate;
        const double C = 1 / P_h * ((k + 1) / (k - dividend->getExpectedIncrease())) * dividend->getDividend();
        const double C1 = C * (1 + dividend->getZ1());
        const double C2 = C * (1 + dividend->getZ2());
        const double E = 2 - riskTolerance;

        EU = A / gamma * (pow((1 - gamma) * B + gamma * C2, E) - pow((1 - gamma) * B + gamma * C1, E));

        return -EU; // returns -EU so that a minimizer can be used instead of a maximizer
    } else if (riiUtility == RIIUtility::logarithmic || riiUtility == RIIUtility::neglogarithmic) {
        /// @todo is this correct?
        const double g = dividend->getExpectedIncrease();

        // definitions as in the bachelor thesis.
        const double A = (1 - gamma) * (1 + interestRate);
        const double B = gamma * (k + 1) * dividend->getDividend() / (P_h * (k - g));
        const double z1 = dividend->getZ1();
        const double z2 = dividend->getZ2();

        EU = log(W_h) +
             1 / (B * (z2 - z1)) *
             ((A + B + B * z2) * (log(A + B + B * z2) - 1) -
              (A + B + B * z1) * (log(A + B + B * z1) - 1));

        if (riiUtility == RIIUtility::neglogarithmic)
            return EU;
        if (riiUtility == RIIUtility::logarithmic)
            return -EU;
    }

    throw "invalid rii strategy.";
}

void AgentLLS::updateBisection(const double &newIterPrice) {
    hypoCash = cash + deltaT->getDeltaT() * ((1 - gamma) * interestRate * cash
                                             + gamma * cash * (newIterPrice - price->getPrice() +
                                                               deltaT->getDeltaT() * dividend->getDividend())
                                               / (deltaT->getDeltaT() * price->getPrice()));
    if (strategy == LLSStrategy::EMB) {
        if (embUtility == EMBUtility::original) {
            // most often, we can find appropriate gammas at the borders and rarely need to optimize.
            double dEULeft = EMBdExpectedUtility(gammaMin);
            double dEURight = EMBdExpectedUtility(gammaMax);

            if (dEULeft < 0)
                iterGamma = gammaMin;
            else if (dEULeft > 0) {
                if (dEURight > 0)
                    iterGamma = gammaMax;
                else if (dEURight < 0) {
                    auto f = [this](double newGamma) -> double { return this->EMBdExpectedUtility(newGamma); };

                    iterGamma = brent::zero(gammaMin, gammaMax, 1e-2, f);
                }
            }
        }
            /// @todo optimize using derivaties, then this section can be removed.
        else if (embUtility == EMBUtility::book) {
            auto f = [this](double newGamma) -> double { return this->EMBexpectedUtility(newGamma); };

            /// @todo Abschätzung zweite Ableitung, hier nur geraten.
            double m = 1e5;
            brent::glomin(gammaMin, gammaMax, 0.0, m, 1e-2, 1e-2, f, iterGamma);
            //brent::local_min(0.01, 0.99, 0.01, f, iterGamma);
        }
    } else if (strategy == LLSStrategy::RII) {
        auto f = [this, newIterPrice](double gamma) -> double {
            return this->RIIexpectedUtility(gamma, newIterPrice, hypoCash);
        };

        /// @todo Abschätzung zweite Ableitung, hier nur geraten.
        double m = 1e5;
        brent::glomin(gammaMin, gammaMax, 0.0, m, 1e-2, 1e-2, f, iterGamma);
        //brent::local_min(0.01, 0.99, 0.01, f, iterGamma);
    } else
        throw "Unknown strategy.";

    updateTradingVolume(iterGamma, newIterPrice, hypoCash);
}

const std::map<std::string, AgentLLS::RIIUtility> AgentLLS::stringToRIIUtility =
        boost::assign::map_list_of("original", AgentLLS::RIIUtility::original)("logarithmic",
                                                                               AgentLLS::RIIUtility::logarithmic)
                ("neglogarithmic", AgentLLS::RIIUtility::neglogarithmic);

const std::map<std::string, AgentLLS::EMBUtility> AgentLLS::stringToEMBUtility =
        boost::assign::map_list_of("original", AgentLLS::EMBUtility::original)("book", AgentLLS::EMBUtility::book);
