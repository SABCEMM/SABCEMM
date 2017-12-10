add_library(PriceCalculator
        PriceCalculator.cpp
        PriceCalculatorBisection.cpp
        PriceCalculatorCross.cpp
        PriceCalculatorFW.cpp
        PriceCalculatorGeneral.cpp
        PriceCalculatorHarras.cpp
        PriceCalculatorHarrasNoise.cpp
        PriceCalculatorLLS.cpp)

add_coverage(PriceCalculator)
