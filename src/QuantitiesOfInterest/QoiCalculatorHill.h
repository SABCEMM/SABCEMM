//
// Created by Simon Cramer on 21.06.18.
//

#ifndef PROJECT_QOICALCULATORHILL_H
#define PROJECT_QOICALCULATORHILL_H

#if BUILD_TESTS
#include "gtest/gtest_prod.h"
#endif


#include "QoiCalculator.h"
#include <vector>
class QoiCalculatorHill: public QoiCalculator {
#if BUILD_TESTS
    //FRIEND_TEST(**, **);
#endif
private:
    double order;
    double singleEstimator(size_t order_, std::vector<double>& data);

public:
    virtual ~QoiCalculatorHill() = default;
    virtual void calculate(std::vector<std::vector<double>> *data);

    QoiCalculatorHill();
    QoiCalculatorHill(double order);

    double getParameter();
};


#endif //PROJECT_QOICALCULATORHILL_H
