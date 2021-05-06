# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

cmake_minimum_required(VERSION 3.12)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(POLICY CMP0077)
  # enable variables set outside to override options
  cmake_policy(SET CMP0077 NEW)
endif()

# Imath version

project(Imath VERSION 3.0.2 LANGUAGES C CXX)

set(IMATH_VERSION_EXTRA "dev" CACHE STRING "Extra version tag string for Imath build")

# See https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
set(IMATH_SOVERSION 28)
set(IMATH_SOAGE 0) 
set(IMATH_SOREVISION 0) 
set(IMATH_LIB_VERSION "${IMATH_SOVERSION}.${IMATH_SOAGE}.${IMATH_SOREVISION}")

set(IMATH_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(IMATH_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(IMATH_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})
set(IMATH_VERSION ${CMAKE_PROJECT_VERSION})
if (NOT IMATH_VERSION_EXTRA STREQUAL "")
  set(IMATH_VERSION "${CMAKE_PROJECT_VERSION}-${IMATH_VERSION_EXTRA}")
endif()
set(IMATH_VERSION_API "${IMATH_VERSION_MAJOR}_${IMATH_VERSION_MINOR}")

message(STATUS "Configure Imath Version: ${IMATH_VERSION} Lib API: ${IMATH_LIB_VERSION}")

# ImathSetup.cmake declares all the configuration variables visible
# in cmake-gui or similar and the rest of the global
# project setup. Check the context to see what is configurable.
include(config/ImathSetup.cmake)

# Config headers and package config files
add_subdirectory(config)

# Utility function for the repeated boilerplate of defining the libraries
include(config/LibraryDefine.cmake)

# Source code is in src/Imath
add_subdirectory(src/Imath)

# Imath_DIR points to the location of ImathConfig.cmake, which tells
# downstream projects where to find Imath, via find_package(Imath).
set(Imath_DIR "${CMAKE_CURRENT_BINARY_DIR}/config" CACHE PATH "" FORCE)

# Add an empty ImathTargets.cmake file for the config to use. It can
# be empty since we already defined the targets in add_subdirectory().
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/config/ImathTargets.cmake" "# Dummy file")

option(PYTHON "Set ON to compile PyImath bindings")
if (PYTHON)
  add_subdirectory(src/python)
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
if(BUILD_TESTING AND NOT IMATH_IS_SUBPROJECT)
  enable_testing()
  add_subdirectory(src/ImathTest)
endif()

# Including this module will add a `clang-format` target to the build
# if the clang-format executable can be found. Only do this if we are
# top level.
if(NOT IMATH_IS_SUBPROJECT)
  include(cmake/clang-format.cmake)
endif()
