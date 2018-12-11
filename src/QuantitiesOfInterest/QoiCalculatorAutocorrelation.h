//
// Created by Simon Cramer on 21.06.18.
//

#ifndef PROJECT_QOICALCULATORAUTOCORRELATION_H
#define PROJECT_QOICALCULATORAUTOCORRELATION_H

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif

#include "QoiCalculator.h"
#include <vector>
class QoiCalculatorAutocorrelation: public QoiCalculator {
#if BUILD_TESTS
    FRIEND_TEST(QoiCalculatorAutoCorrelation, singleEstimator);
    FRIEND_TEST(QoiCalculatorAutoCorrelation, calculate);
    FRIEND_TEST(QoiCalculatorAutoCorrelation, constructor);
#endif
private:
    size_t tau;
    double singleEstimator(size_t tau_, std::vector<double> &data, double &gamma_0, double &mean);

public:
    virtual ~QoiCalculatorAutocorrelation() = default;
    virtual void calculate(std::vector<std::vector<double>> *data);

    QoiCalculatorAutocorrelation();
    QoiCalculatorAutocorrelation(size_t tau);

    double getParameter();
};


#endif //PROJECT_QOICALCULATORAUTOCORRELATION_H
