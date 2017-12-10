FILE(GLOB_RECURSE inputFiles "*.xml")
# make input files visible in IDEs
add_custom_target(input SOURCES ${inputFiles})
install(DIRECTORY . DESTINATION share/SABCEMM/examples
    FILES_MATCHING PATTERN "*.xml")
