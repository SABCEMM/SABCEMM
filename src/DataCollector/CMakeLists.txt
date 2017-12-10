add_library(DataCollector
        DataCollector.cpp
        DataItemCollectorAmountOfCash.cpp
        DataItemCollectorAmountOfStock.cpp
        DataItemCollector.cpp
        DataItemCollectorExcessDemand.cpp
        DataItemCollectorFWShares.cpp
        DataItemCollectorHarrasK.cpp
        DataItemCollectorEMBGamma.cpp
        DataItemCollectorPrice.cpp
        DataItemCollectorWealth.cpp
)

add_coverage(DataCollector)

add_dependencies(DataCollector
        Group)

target_link_libraries(DataCollector
        Group
)
