add_library(Parameter
        ParameterSetAgent.cpp
        ParameterSetExcessDemandCalculator.cpp
        ParameterSetPriceCalculator.cpp
        ParameterSetRNG.cpp
        ParameterSetDataItemCollector.cpp
        ParameterSetDividend.cpp
        Parameter.cpp)

target_link_libraries( Parameter tinyxml2 DataCollector)

add_coverage(Parameter)
