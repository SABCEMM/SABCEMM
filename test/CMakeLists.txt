if(${BUILD_TESTS})
add_subdirectory(AgentTest)
add_subdirectory(DataCollectorTest)
add_subdirectory(DummyClasses)
add_subdirectory(ExcessDemandCalculatorTest)
add_subdirectory(GroupTest)
add_subdirectory(MockClasses)
add_subdirectory(NeighbourhoodGeneratorTest)
add_subdirectory(ParameterTest)
add_subdirectory(PriceCalculatorTest)
add_subdirectory(RandomGeneratorTest)
add_subdirectory(FullSimulationTest)
add_subdirectory(StockExchangeTest)
add_subdirectory(VariableContainerTest)
add_subdirectory(WriterTest)

# taken from: http://stackoverflow.com/questions/8507723/how-to-start-working-with-gtest-and-cmake
enable_testing()

#TODO: The Main-library should be linked in, but that does not work (presumably because of the '-Wl'-statements)
add_executable(financeSimulationTests mainTest.cpp ../src/Simulation.cpp)

# quite hacky command line, see here: http://stackoverflow.com/questions/17912988/gtest-not-finding-tests-in-separate-compilation-units
if(${APPLE})
    target_link_libraries(financeSimulationTests Agent Algorithms DataCollector ExcessDemandCalculator Group
            NeighbourhoodGenerator Parameter PriceCalculator RandomGenerator StockExchange VariableContainer Writer
                            tinyxml2
                            gtest gmock
                            -Wl,-force_load AgentTest -Wl,-force_load DataCollectorTest -Wl,-force_load GroupTest
            -Wl,-force_load DummyClasses -Wl,-force_load ExcessDemandCalculatorTest
            -Wl,-force_load NeighbourhoodGeneratorTest -Wl,-force_load ParameterTest
            -Wl,-force_load PriceCalculatorTest -Wl,-force_load RandomGeneratorTest
            -Wl,-force_load FullSimulationTest -Wl,-force_load VariableContainerTest
            -Wl,-force_load WriterTest
            -Wl,-force_load StockExchangeTest
                            Version
    )
else()
    target_link_libraries(financeSimulationTests Agent Algorithms DataCollector ExcessDemandCalculator Group
            NeighbourhoodGenerator Parameter PriceCalculator RandomGenerator StockExchange VariableContainer
            Version Writer
                            tinyxml2
                            gtest gmock
                            -Wl,--whole-archive AgentTest DataCollectorTest GroupTest DummyClasses
            ExcessDemandCalculatorTest NeighbourhoodGeneratorTest ParameterTest PriceCalculatorTest RandomGeneratorTest
            FullSimulationTest VariableContainerTest WriterTest StockExchangeTest
            -Wl,-no-whole-archive
                                        Version # linked lastly as WriterTest depends on it.
    )

endif()

if(${CURSES_FOUND})
    target_link_libraries(financeSimulationTests progressbar ${CURSES_LIBRARIES})
endif(${CURSES_FOUND})

if(${WITH_HDF5})
    target_link_libraries(financeSimulationTests ${HDF5_LIBRARIES})
endif(${WITH_HDF5})

if(${WITH_INTEL_MKL})
	target_link_libraries(financeSimulationTests ${MKL_LIBRARIES})
endif(${WITH_INTEL_MKL})

if(${WITH_NAG})
        target_link_libraries(financeSimulationTests ${NAG_LIBRARIES})
endif(${WITH_NAG})

add_test(financeSimulationTests financeSimulationTests)

endif(${BUILD_TESTS})

