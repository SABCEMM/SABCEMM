add_library(tinyxml2
	tinyxml2.cpp
	tinyxml2.h
)

SET(TINYXML2_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/.. PARENT_SCOPE)

message(STATUS ${TINYXML2_INCLUDES})
