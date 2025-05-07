# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(POLICY CMP0077)
  # enable variables set outside to override options
  cmake_policy(SET CMP0077 NEW)
endif()

# Imath version

project(Imath VERSION 3.2.0 LANGUAGES C CXX)

set(IMATH_VERSION_RELEASE_TYPE "-dev" CACHE STRING "Extra version tag string for Imath build, such as -dev, -beta1, etc.")

set(IMATH_VERSION ${Imath_VERSION})
set(IMATH_VERSION_API "${Imath_VERSION_MAJOR}_${Imath_VERSION_MINOR}")

# The SOVERSION (i.e. numerical component of SONAME) tracks the ABI
# version. Increment this number whenever, and only when, the ABI changes in
# non-backwards-compatible ways.
#
# The OpenEXR project policy is to append the library version
# "major.minor.patch" to the SONAME to form the real shared library name.
# For example, in "libImath.so.31.3.2.0", "libImath.so.31" is the SONAME
# and ".3.2.0" identifies the corresponding library release.

set(IMATH_LIB_SOVERSION 30)
set(IMATH_LIB_VERSION "${IMATH_LIB_SOVERSION}.${IMATH_VERSION}") # e.g. "30.3.2.0"

# ImathSetup.cmake declares all the configuration variables visible
# in cmake-gui or similar and the rest of the global
# project setup. Check the context to see what is configurable.
include(config/ImathSetup.cmake)

message(STATUS "Configure ${IMATH_PACKAGE_NAME}, library API version: ${IMATH_LIB_VERSION}")

option(IMATH_INSTALL "Install Imath library" ON)

if (APPLE)
  option(IMATH_BUILD_APPLE_FRAMEWORKS "Build as Apple Frameworks" OFF)
endif()

# Config headers and package config files
add_subdirectory(config)

if (IMATH_BUILD_APPLE_FRAMEWORKS)
  # Define resource files for Apple Framework
  set(IMATH_RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/README.md"
    "${CMAKE_SOURCE_DIR}/LICENSE.md"
    "${CMAKE_SOURCE_DIR}/SECURITY.md"
    "${CMAKE_CURRENT_BINARY_DIR}/config/ImathTargets.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/config/ImathConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/config/ImathConfigVersion.cmake"
  )
  if (PYTHON)
    set(IMATH_RESOURCE_FILES ${IMATH_RESOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/config/PyImathTargets.cmake")
  endif()
  if (PYBIND11)
    set(IMATH_RESOURCE_FILES ${IMATH_RESOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/config/PyBindImathTargets.cmake")
  endif()

  message(STATUS "Resource files: ${IMATH_RESOURCE_FILES}")
endif()

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Source code is in src/Imath
add_subdirectory(src/Imath)

# Imath_DIR points to the location of ImathConfig.cmake, which tells
# downstream projects where to find Imath, via find_package(Imath).
set(Imath_DIR "${CMAKE_CURRENT_BINARY_DIR}/config" CACHE PATH "" FORCE)

option(PYTHON "Set ON to compile boost PyImath bindings")
if (PYTHON)
  add_subdirectory(src/python)
endif()

option(PYBIND11 "Set ON to compile pybind11 PyImath bindings")
if (PYBIND11)
  add_subdirectory(src/pybind11)
endif()

option(BUILD_WEBSITE "Set ON to build the readthedocs website source")
if (BUILD_WEBSITE AND NOT IMATH_IS_SUBPROJECT)
  add_subdirectory(website)
endif()

# pkg-config is not required for Apple Frameworks
if (IMATH_BUILD_APPLE_FRAMEWORKS)
  set(IMATH_INSTALL_SYM_LINK OFF)
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

