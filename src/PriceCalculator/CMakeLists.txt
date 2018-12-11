add_library(PriceCalculator
        PriceCalculator.cpp
        PriceCalculatorBisection.cpp
        PriceCalculatorCross.cpp
        PriceCalculatorFW.cpp
        PriceCalculatorGeneral.cpp
        PriceCalculatorHarras.cpp
        PriceCalculatorHarrasNoise.cpp
        PriceCalculatorLLS.cpp
        PriceCalculatorLLSNoise.cpp
        PriceCalculatorLLS1.cpp
        )

add_coverage(PriceCalculator)
