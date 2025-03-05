# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(GNUInstallDirs)

#
# Python and Boost
#

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Could not find Python")
endif()

#
# Python library suffix string, by default including python version and
# Imath release number, e.g. "_Python3_12-3_4"
#

set(PYBINDIMATH_LIB_SUFFIX
    "_Python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}-${IMATH_VERSION_API}"
    CACHE STRING
    "Suffix string for python version and Imath release in library names"
)


#
# PyBindImath
#

add_subdirectory(PyBindImath)

#
# Tests
#

include(CTest)

# This option allows the tests to be built but not run, helpful to
# catch build failures even if the test itself fails

option(IMATH_TEST_PYBIND11 "Include the pybind11 tests when BUILD_TESTING is on" ON)

if(BUILD_TESTING AND IMATH_TEST_PYBIND11)
  enable_testing()
  add_subdirectory( PyBindImathTest )
endif()

