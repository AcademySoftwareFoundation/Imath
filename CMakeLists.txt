# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

if(IMATH_BUILD_BOTH_STATIC_SHARED OR IMATH_BUILD_BOTH_STATIC_SHARED)
  if (${CMAKE_VERSION} VERSION_LESS "3.12.0")
    message(FATAL_ERROR "CMake 3.12 or newer is required for object library support when building both static and shared libraries")
  endif()
  cmake_minimum_required(VERSION 3.12)
else()
  cmake_minimum_required(VERSION 3.10)
endif()

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# we include this first to obtain version numbers from version.cmake
include(config/version.cmake)

#set up our project
project(Imath VERSION ${IMATH_VERSION} LANGUAGES C CXX)

# An "official" way to make this a super-project
# basically overrides the find_package to not find anything
# for stuff we're including locally
set(as_subproject Imath)
macro(find_package)
  if(NOT "${ARGV0}" IN_LIST as_subproject)
    _find_package(${ARGV})
  endif()
endmacro()

#####################n##################
#######################################
# This declares all the configuration variables visible
# in cmake-gui or similar and the rest of the global
# project setup
#
# Please look at this file to see what is configurable
#######################################
#######################################
include(config/ImathSetup.cmake)

# generates config headers, package config files
add_subdirectory(config)

# utility function for the repeated boilerplate of defining
# the libraries
include(config/LibraryDefine.cmake)

#######################################

# Include these modules without enable/disable options
add_subdirectory(Half)
add_subdirectory(Imath)

if(BUILD_PYIMATH)
  find_path(PYIMATH PyImath)
  if((NOT PYIMATH) OR (NOT EXISTS ${PYIMATH}))
    # PyImath has not yet been cloned as a submodule
    execute_process(COMMAND git submodule update --init -- ${PYIMATH}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
  add_subdirectory(PyImath)
endif()

# If you want to use ctest to configure, build and
# upload the results, cmake has builtin support for
# submitting to CDash, or any server who speaks the
# same protocol
# 
# These settings will need to be set for your environment,
# and then a script such as the example in
#
# cmake/SampleCTestScript.cmake
#
# edited and placed into the CI system, then run:
#
# cmake -S cmake/SampleCTestScript.cmake
#
# [or whatever you name the file you edit]
# 
#set(CTEST_PROJECT_NAME "Imath")
#set(CTEST_NIGHTLY_START_TIME "01:01:01 UTC")
#set(CTEST_DROP_METHOD "http") # there are others...
#set(CTEST_DROP_SITE "open.cdash.org")
#set(CTEST_DROP_LOCATION "/submit.php?project=MyProject")
#set(CTEST_DROP_SITE_CDASH TRUE)

include(CTest)
if(BUILD_TESTING)
  enable_testing()

  add_subdirectory(HalfTest)
  add_subdirectory(ImathTest)
endif()

# Including this module will add a `clang-format` target to the build if
# the clang-format executable can be found.
include(cmake/clang-format.cmake)
