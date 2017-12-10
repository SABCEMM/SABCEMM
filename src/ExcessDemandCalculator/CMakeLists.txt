add_library(ExcessDemandCalculator
        ExcessDemandCalculator.cpp
        ExcessDemandCalculatorFW.cpp
        ExcessDemandCalculatorHarras.cpp
        ExcessDemandCalculatorLLS.cpp
)

add_coverage(ExcessDemandCalculator)
