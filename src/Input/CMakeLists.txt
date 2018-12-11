add_library(Input
        Input.cpp)

target_link_libraries( Input tinyxml2 DataCollector)

add_coverage(Input)
