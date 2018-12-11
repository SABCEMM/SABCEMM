#ifndef AGENTLLS_H
#define AGENTLLS_H

#include "Agent.h"
#include <boost/assign.hpp>
#include <map>
#include <set>



///
/// @brief The AgentLLS class
///
/// @todo Provides a custom implementation of hasGroup, for dynamic gorups
///
class AgentLLS : public Agent {
#if BUILD_TESTS
        FRIEND_TEST(fullSimulationTest, fullSimulation_RII);
        FRIEND_TEST(fullSimulationTest, fullSimulation_EMB);
#endif
public:
    enum class LLSStrategy {
        EMB, /// efficient market believer
        RII /// rational identical
    };

    /// See the LLS chapter in Max bachelor thesis for more information.
    enum class RIIUtility {
        original, /// utility function as proposed in the book by Levy (2000)
        logarithmic,   /// logarithmic utility function, inspired by the paper by Levy (1995)
        neglogarithmic
    };

    enum class EMBUtility {
        original, /// paper by Levy (1995)
        book /// book by Levy(2000)
    };

    enum class MemorySpanMode{
        fixed,
        random
    };

    static const std::map<std::string, RIIUtility> stringToRIIUtility;

    static const std::map<std::string, EMBUtility> stringToEMBUtility;

    AgentLLS(RandomGenerator *newRandomGenerator,
             Price *newPrice,
             double newCash,
             double newStock,
             Dividend *newDividend,
             double newRiskTolerance,
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
             std::size_t memorySpan,
             double memorySpanMu,
             double memorySpanSigma);

    double getOldGamma() { return oldGamma; }
    double getGamma() { return gamma; }
    double getIterGamma() { return iterGamma; }
    double getInterestRate() { return interestRate; }
    double getStock() { return stock; }
    std::size_t getMemorySpan() { return memorySpan; }

    bool hasGroup(int groupID) override {
        if (Group::hasGroup(groupID))
            return true;
        return currentStrategyGroup.find(groupID) != currentStrategyGroup.end();
    }

    void setStrategy(LLSStrategy strategy) {
        this->strategy = strategy;
        switch (this->strategy) {
            case LLSStrategy::EMB:
                this->currentStrategyGroup = embGroup;
                break;
            case LLSStrategy::RII:
                this->currentStrategyGroup = riiGroup;
        }
    }

    LLSStrategy getStrategy() { return strategy; }

    void toggleStrategy() {
        if (strategy == LLSStrategy::EMB)
            setStrategy(LLSStrategy::RII);
        else
            setStrategy(LLSStrategy::EMB);
    }

private:
    // the set of groups that is active due to current strategy.
    // Is always identical to either embGroup or riiGroup.
    std::set<int> currentStrategyGroup;

    double riskTolerance; //Alpha in the Utility Function

    double stdNoiseSigma; //Standard deviation for the noise on sigma
    double interestRate;

    double gamma;
    double iterGamma;
    double oldGamma;
    double hypoCash;

    AgentLLS::LLSStrategy strategy;
    // RII variables and functions
    double k;

    double RIIexpectedUtility(double gamma, double P_h, double W_h);

    /// @todo maybe we should outsource this calculation into a ExpectedUtilityCalculator?
    AgentLLS::RIIUtility riiUtility;
    const std::set<int> riiGroup;


    // EMB variables and functions
    AgentLLS::EMBUtility embUtility;
    const std::set<int> embGroup;
    std::size_t memorySpan; //How far does the agent look back

    /// @todo replace with a deque for pop_front (faster!), that requires making
    ///       RandomGenerator::getUniformRandomDouble(...) a template.
    /// relative stock price change time series
    std::vector<double> relPriceHistory;
    double EMButility(double newCash);

    double EMBexpectedUtility(double newGamma);

    /// first derivative of EU.
    double EMBdExpectedUtility(double gamma);


    Dividend *dividend; /**< Pointer to the dividend container */

    double oldPrice;
    double gammaMin, gammaMax;

    void preStepUpdate() override {}
    void stepUpdate() override;
    void postStepUpdate() override;

    void updateCash();

    void updateBisection(const double &newIterPrice) override;
    void updateTradingVolume(double newGamma, double newPrice, double newHypoCash);
};

#endif // AGENTLLS_H
