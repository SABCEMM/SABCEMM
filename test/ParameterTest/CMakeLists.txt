# copy xml-files to build-folder
file(GLOB xmlFiles *.xml)
foreach(xmlFile ${xmlFiles})
	configure_file(${xmlFile} ${CMAKE_BINARY_DIR}/test/ParameterTest)
endforeach()

# make input files visible in IDEs
add_custom_target(ParameterTestInput SOURCES ${xmlFiles})


add_library(ParameterTest ParameterTest.cpp ParameterSetRNGTest.cpp)
target_link_libraries(ParameterTest Parameter)
