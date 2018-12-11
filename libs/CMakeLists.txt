add_subdirectory(tinyxml2)
LIST(APPEND _LIB_INCLUDES ${TINYXML2_INCLUDES})

if(${BUILD_TESTS})
	# googletest will ship its own CMakeLists.txt(!)
	add_subdirectory(googletest)
	mark_as_advanced(gmock_build_tests gtest_build_tests gmock_build_samples gtest_build_samples gmock_build_tests gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols)
	LIST(APPEND _LIB_INCLUDES ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
endif()

if(${CURSES_FOUND})
    include_directories(progressbar/include/progressbar) # needed for the compilation of progressbar
    add_subdirectory(progressbar/lib)
    LIST(APPEND _LIB_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/progressbar/include)
else()
    message("Could not find ncurses. Lib progressbar will be disabled.")
endif(${CURSES_FOUND})

SET(LIB_INCLUDES ${_LIB_INCLUDES} PARENT_SCOPE)
