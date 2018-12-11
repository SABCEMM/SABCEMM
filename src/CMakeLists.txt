add_subdirectory(Agent)
add_subdirectory(Algorithms)
add_subdirectory(DataCollector)
add_subdirectory(Exceptions)
add_subdirectory(ExcessDemandCalculator)
add_subdirectory(Group)
add_subdirectory(NeighbourhoodGenerator)
add_subdirectory(Input)
add_subdirectory(PriceCalculator)
add_subdirectory(QuantitiesOfInterest)
add_subdirectory(RandomGenerator)
add_subdirectory(StockExchange)
add_subdirectory(Switching)
add_subdirectory(Util)
add_subdirectory(VariableContainer)
add_subdirectory(Writer)
add_subdirectory(Version)

add_library(Main
        Simulation.cpp
    )

#target_compile_definitions(Main PUBLIC -DWITH_PROGBAR=$<BOOL:${CURSES_FOUND}>)

add_executable (financeSimulation FinanceSimulation.cpp)
install(TARGETS financeSimulation
    RUNTIME DESTINATION bin
    )

target_link_libraries(financeSimulation
        Main
        Agent
        Algorithms
        DataCollector
        ExcessDemandCalculator
        NeighbourhoodGenerator
        Input
        PriceCalculator
        QuantitiesOfInterest
        RandomGenerator
        StockExchange
        Switching
        VariableContainer
        Writer
        tinyxml2
        Version # linked lastly as Writer depends on it.
)

target_link_libraries(financeSimulation ${Boost_LIBRARIES})

if(${WITH_PROGBAR})
if(CURSES_FOUND)
    target_link_libraries(financeSimulation progressbar ${CURSES_LIBRARIES})
endif(CURSES_FOUND)
endif()

if(${WITH_INTEL_MKL})
    target_link_libraries(financeSimulation ${MKL_LIBRARIES})
#    if(${INTEL_MKL_PARALLEL})
#        target_link_libraries(financeSimulation ${MKL_LP_GNUTHREAD_LIBRARIES})
#    else(${INTEL_MKL_PARALLEL})
#        target_link_libraries(financeSimulation ${MKL_LP_SEQUENTIAL_LIBRARIES})
#    endif(${INTEL_MKL_PARALLEL})
endif(${WITH_INTEL_MKL})

if(${WITH_NAG})
	target_link_libraries(financeSimulation ${NAG_LIBRARIES})
endif(${WITH_NAG})

if(${WITH_HDF5})
    target_link_libraries(financeSimulation ${HDF5_LIBRARIES})
endif(${WITH_HDF5})
