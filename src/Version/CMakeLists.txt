include(GetGitRevisionDescription)

get_git_head_revision(GIT_REFSPEC GIT_SHA1)
# passing --dirty here would be neat, but does not work without patching rpavlik's modules.
git_describe(GIT_VERSION --always)


cmake_host_system_information(RESULT FQDN QUERY FQDN)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/Version.cpp" @ONLY)

add_library(Version
        Version.h
	${CMAKE_CURRENT_BINARY_DIR}/Version.cpp
)

# TODO: build fails, if the following line is not commented. Why? Maybe as Version.cpp does not define functions?
#add_coverage(Version)
