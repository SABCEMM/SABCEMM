add_library(QuantitiesOfInterest
        QuantitiesOfInterest.cpp
        QuantityOfInterest.cpp
        QoiCalculator.cpp
        QoiCalculatorExcessKurtosis.cpp
        QoiCalculatorKurtosis.cpp
        QoiCalculatorMaximum.cpp
        QoiCalculatorMean.cpp
        QoiCalculatorMinimum.cpp
        QoiCalculatorMoment.cpp
        QoiCalculatorQuantile.cpp
        QoiCalculatorSkew.cpp
        QoiCalculatorVariance.cpp
        QoiCalculatorFull.cpp
        QoiCalculatorHill.cpp
        QoiCalculatorAutocorrelation.cpp)

add_coverage(QuantitiesOfInterest)

add_dependencies(QuantitiesOfInterest
        DataCollector
        Group
        )

target_link_libraries(QuantitiesOfInterest
        DataCollector
        Group

)
