add_library(DataCollector
        DataCollector.cpp
        DataItemCollectorAmountOfCash.cpp
        DataItemCollectorAmountOfStock.cpp
        DataItemCollector.cpp
        DataItemCollectorExcessDemand.cpp
        DataItemCollectorFWSharesChartist.cpp
        DataItemCollectorFWSharesFundamentalist.cpp
        DataItemCollectorHarrasK.cpp
        DataItemCollectorLLSMemorySpans.cpp
        DataItemCollectorEMBGamma.cpp
        DataItemCollectorPrice.cpp
        DataItemCollectorSwitchableShares.cpp
        DataItemCollectorWealth.cpp
)

add_coverage(DataCollector)

add_dependencies(DataCollector
        Group
        )

target_link_libraries(DataCollector
        Group

)
