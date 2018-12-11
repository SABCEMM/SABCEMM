 # copy xml-files to build-folder
file(GLOB xmlFiles *.xml)
foreach(xmlFile ${xmlFiles})
	configure_file(${xmlFile} ${CMAKE_BINARY_DIR}/test/InputTest)
endforeach()

# make input files visible in IDEs
add_custom_target(InputTestInput SOURCES ${xmlFiles})


add_library(InputTest InputTest.cpp)
target_link_libraries(InputTest Input)
