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

find_package(Boost REQUIRED COMPONENTS python)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()

#
# Python library suffix string, by default including python version and
# Imath release number, e.g. "_Python3_12-3_4"
#

set(PYIMATH_LIB_SUFFIX
    "_Python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}-${IMATH_VERSION_API}"
    CACHE STRING
    "Suffix string for python version and Imath release in library names"
)

#
# PyImath and PyImathNumpy
#

add_subdirectory( PyImath )
add_subdirectory( PyImathNumpy )

#
# Tests
#

include(CTest)

# This option allows the tests to be built but not run, helpful to
# catch build failures even if the test itself fails

option(IMATH_TEST_PYTHON "Include the python tests when BUILD_TESTING is on" ON)

if(BUILD_TESTING AND IMATH_TEST_PYTHON)
  enable_testing()
  add_subdirectory( PyImathTest )
  add_subdirectory( PyImathPythonTest )
  add_subdirectory( PyImathNumpyTest )
endif()
