FILE(GLOB_RECURSE headerFiles "*.h")
# make input files visible in IDEs
add_custom_target(headers SOURCES ${headerFiles})
