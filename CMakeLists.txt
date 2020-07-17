# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

if(IMATH_BUILD_BOTH_STATIC_SHARED)
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

#######################################
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
# Tell CMake where to find the ImathConfig.cmake file. Makes it posible to call 
# find_package(Imath) in downstream projects
set(Imath_DIR "${CMAKE_CURRENT_BINARY_DIR}/config" CACHE PATH "" FORCE)
# Add an empty ImathTargets.cmake file for the config to use. 
# Can be empty since we already defined the targets in add_subdirectory
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/config/ImathTargets.cmake" "# Dummy file")


option(PYTHON "Set ON to compile PyImath bindings")
if (PYTHON)
  add_subdirectory(python)
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
