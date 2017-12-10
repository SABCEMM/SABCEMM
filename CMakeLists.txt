cmake_minimum_required(VERSION 2.8.12)
# cmake modules
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/rpavlik")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/Eyescale")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/RWTH-HPC/cmake")

include(FeatureSummary)
include(CMakeDependentOption)

SET(BUILD_TESTS ON CACHE BOOL "Build SABCEMM-tests.")
add_feature_info(Tests BUILD_TESTS "Tests will only be built if BUILD_TESTS is ON.")
SET(ENABLE_RNGSPEEDTEST OFF CACHE BOOL "Enable speed-measurement for RNG during testing. Runtime > 10h")

mark_as_advanced(BUILD_GMOCK)
mark_as_advanced(BUILD_GTEST)
SET(BUILD_GMOCK ${BUILD_TESTS} CACHE BOOL "" FORCE)
SET(BUILD_GTEST ${BUILD_TESTS} CACHE BOOL "" FORCE)


find_package(Curses)
if(CURSES_FOUND)
include_directories(${CURSES_INCLUDE_DIRS})
set_package_properties(Curses PROPERTIES TYPE OPTIONAL
                                              PURPOSE "Needed by the progressbar lib.")
endif()

add_subdirectory(libs)

# todo: replace with target_compile_features() (needs newer CMake)
add_compile_options(-std=c++11)
add_compile_options(-Wall -Wextra -Wconversion -Wparentheses -Wunreachable-code -Wredundant-decls -Wcast-align -Wfloat-equal -Wno-long-long -Wformat=2 -pedantic)

SET(WITH_INTEL_MKL OFF CACHE BOOL "Needed for the IntelMKL-random generator")
add_feature_info("Intel MKL RNG" WITH_INTEL_MKL "The MKL RNG will only be available if WITH_INTEL_MKL is ON.")
SET(WITH_NAG OFF CACHE BOOL "Needed for the NAG-random generator")
add_feature_info("NAG RNG" WITH_NAG "The NAG RNG will only be available if WITH_NAG is ON.")
SET(WITH_HDF5 OFF CACHE BOOL "Needed for HDF5-support")
add_feature_info("HDF5 support" WITH_HDF5 "HDF5 support is enabled if WITH_HDF5 is ON. WriterHDF5 will only be available with HDF5 support.")

# global, may be made local using target_compile_definitions.
# global should be fine though.
add_definitions(
        -DWITH_HDF5=$<BOOL:${WITH_HDF5}>
        -DWITH_INTEL_MKL=$<BOOL:${WITH_INTEL_MKL}>
        -DWITH_NAG=$<BOOL:${WITH_NAG}>
        -DBUILD_TESTS=$<BOOL:${BUILD_TESTS}>
        -DENABLE_RNGSPEEDTEST=$<BOOL:${ENABLE_RNGSPEEDTEST}>)

# LIB_INCLUDES is set by "libs"-CMakeLists.
# making those "SYSTEM" libraries suppresses warnings.
include_directories(SYSTEM ${LIB_INCLUDES})

# do not omit CXX, or only the C-bindings will be found
if(${WITH_HDF5})
    find_package(HDF5 COMPONENTS CXX REQUIRED)
    include_directories(${HDF5_INCLUDE_DIRS})
endif()

if(${WITH_INTEL_MKL})
	find_package(MKL REQUIRED)
	include_directories(${MKL_INCLUDE_DIR})
	SET(INTEL_MKL_PARALLEL OFF CACHE BOOL "If true, MKL will be linked with OpenMP threading. Otherwise, MKL will be linked as sequential library.")
	if(${INTEL_MKL_PARALLEL})
		find_package(OpenMP REQUIRED)
		set_package_properties(OpenMP PROPERTIES TYPE REQUIRED
                                              PURPOSE "Needed for Intel MKL with OpenMP threading.")
		if(OPENMP_FOUND)
			SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
		endif(OPENMP_FOUND)
	endif(${INTEL_MKL_PARALLEL})
endif(${WITH_INTEL_MKL})

if(${WITH_NAG})
	find_package(NAG REQUIRED)
	include_directories(${NAG_INCLUDE_DIRS})
endif(${WITH_NAG})

# set up RWTH-HPC code coverage module.
find_package(codecov)
list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/libs/*'")

add_subdirectory(src)
add_subdirectory(test)

add_subdirectory(input)

coverage_evaluate()

feature_summary(WHAT ALL)
