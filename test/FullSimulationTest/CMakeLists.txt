# copy xml-files to build folder
file(GLOB inputFiles *.xml *.csv)
foreach(inputFile ${inputFiles})
        configure_file(${inputFile} ${CMAKE_BINARY_DIR}/test/FullSimulationTest)
endforeach()

# make input files visible in IDEs
add_custom_target(FullSimulationInput SOURCES ${inputFiles} )


add_library(FullSimulationTest fullSimulationTest.cpp)
