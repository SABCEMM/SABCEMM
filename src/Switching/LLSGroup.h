#ifndef LLSGROUP_H
#define LLSGROUP_H

#include "Switchable.h"
#include "../Agent/AgentLLS.h"
#include "../RandomGenerator/RandomGenerator.h"
#include <vector>
#include <random>

class LLSGroup : public Switchable {
private:
    double alpha_p1;
    double alpha_p2;

    std::vector<AgentLLS *> LLSagents;
    RandomGenerator &rng;
    Dividend &dividend;
    Price &price;
    double discountFactor;
public:
    LLSGroup(RandomGenerator &rng, Dividend &dividend, double discountFactor, Price &price, double alpha_p1,
             double alpha_p2) :
            alpha_p1(alpha_p1),
            alpha_p2(alpha_p2),
            rng(rng),
            dividend(dividend),
            price(price),
            discountFactor(discountFactor) {}

    virtual void setChartistShare(double chartistShare) {
        size_t newChartistCount = (size_t) round((double) LLSagents.size() * chartistShare);

        size_t chartistCount = getChartistCount();


        // shuffling needs to be coupled to the RNG to ensure reproducability for
        // equal seeds.
        /// @todo this code was copied, unify it.
        int seed = 0;
        rng.getUniformRandomInt(0, std::numeric_limits<int>::max(), &seed);
        std::mt19937 g(static_cast<unsigned int>(seed));
        std::shuffle(LLSagents.begin(), LLSagents.end(), g);

        long missingChartists = static_cast<long>(newChartistCount - chartistCount);

        for (auto a : LLSagents) {
            if (missingChartists > 0) {
                if (a->getStrategy() == AgentLLS::LLSStrategy::RII) {
                    a->toggleStrategy();
                    --missingChartists;
                }
            } else if (missingChartists < 0) {
                if (a->getStrategy() == AgentLLS::LLSStrategy::EMB) {
                    a->toggleStrategy();
                    ++missingChartists;
                }
            }
        }


    }

    virtual double getalpha_p1() { return alpha_p1; }

    virtual double getalpha_p2() { return alpha_p2; }

    virtual double getChartistShare() { return (double) getChartistCount() / (double) LLSagents.size(); }

    /// @todo make this a variable, if necessary.
    /// taken from the Franke-Westerhoff paper for DCA-HPM, table 1
    virtual double getPredisposition() { return -0.327; }

    /// taken from Levy, Levy, Solomon: "Microscopic Simulation of Financial Markets", p.149
    virtual double getFundamentalPrice() {
        /// @todo introduce a switch here - which versions are the best working ones?
        /// @todo If I remember correctly, the version with the initialPrice is not sensible
        /// and the other ones are both ok. Needs further testing.
        double res;

        static size_t counter = 0;
        static double initialDividend = -1;
        static double initialPrice = -1;

        if (counter == 0) {
            initialDividend = dividend.getDividend();
            initialPrice = price.getPrice();
        }
        ++counter;


        const double k = 0.01;
        res = initialPrice * 0.1 * exp(k * (double) counter);

        // equal to the expected increase here, see p.157
        //const double g = dividend.getExpectedIncrease();
        //res = dividend.getDividend()*(1+dividend.getExpectedIncrease())*(1+g)/(discountFactor-g);

        // Version 3
        const size_t numAgents = LLSagents.size();
        const size_t numStock = numAgents * 100;
        const double avgGamma = 0.8;
        double avgWealth = 0;
        const double interestRate = 0.01;
        const double lastPrice = price.getPrice();
        const double expectedDividend = dividend.getDividend() * (1 + dividend.getExpectedIncrease());

        for (AgentLLS *a : LLSagents)
            avgWealth += a->getCash();

        avgWealth /= (double) LLSagents.size();

        res = numAgents * avgGamma / numStock * (
                avgWealth +
                (1 - avgGamma) * interestRate * avgWealth +
                avgWealth / lastPrice * (lastPrice + expectedDividend)) /
              (1 - 0.8 * avgWealth / numStock / lastPrice);

        return res;
    }

    void addToGroup(AgentLLS *a) { LLSagents.push_back(a); }

private:
    size_t getChartistCount() {
        size_t chartistCount = 0;

        for (auto a : LLSagents)
            if (a->getStrategy() == AgentLLS::LLSStrategy::EMB)
                ++chartistCount;

        return chartistCount;
    }
};

#endif // LLSGROUP_H
