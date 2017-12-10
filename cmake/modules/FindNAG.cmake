# - Try to find the NAG library.
#   based on the FindMKL-module.
# Once done this will define
#
# NAG_FOUND - system has NAG
# NAG_ROOT_DIR - path to the NAG base directory
# NAG_INCLUDE_DIR - the NAG include directory
# NAG_LIBRARIES - NAG libraries
#

find_path(NAG_ROOT_DIR include/nag.h HINTS
  /opt/nag/*/
  )

include_directories("${NAG_ROOT_DIR}/include")

find_path(NAG_INCLUDE_DIR
  nag.h
  PATHS
    ${NAG_ROOT_DIR}/include
)

find_library(NAG_CORE_LIBRARY
  nagc_nag
  PATHS
    ${NAG_ROOT_DIR}/lib
)

find_library(NAG_IFCOREMT_LIBRARY
  ifcoremt
  PATHS
    ${NAG_ROOT_DIR}/rtl/intel64
)

set(NAG_LIBRARY ${NAG_CORE_LIBRARY} ${NAG_IFCOREMT_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NAG DEFAULT_MSG NAG_INCLUDE_DIR NAG_LIBRARY)

mark_as_advanced(
  NAG_INCLUDE_DIR
  NAG_CORE_LIBRARY
  NAG_IFCOREMT_LIBRARY
    )


set(NAG_LIBRARIES ${NAG_LIBRARY})
set(NAG_INCLUDE_DIRS ${NAG_INCLUDE_DIR})
