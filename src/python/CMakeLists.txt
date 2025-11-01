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

find_package(Boost CONFIG REQUIRED COMPONENTS python)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()

if (NOT DEFINED PYTHON_INSTALL_DIR)

  #
  # Set the PYTHON_INSTALL_DIR but only if it's not already set,
  # which allows an externally-set value to take effect.
  #
  # PYTHON_INSTALL_DIR should be a relative path underneath the 
  # INSTALL_DIR. Set it to Python3_SITEARCH, relative to the python
  # system installation.
  #
    
  if (NOT Python3_PREFIX)
    # if Python3_PREFIX is not set, use the location of the python executable
    get_filename_component(Python3_PREFIX "${Python3_EXECUTABLE}" DIRECTORY)
    get_filename_component(Python3_PREFIX "${Python3_PREFIX}" DIRECTORY)
  endif()

  file(RELATIVE_PATH PYTHON_INSTALL_DIR
    "${Python3_PREFIX}"
    "${Python3_SITEARCH}")

endif()

#
# Python library suffix string. By default, it includes the python
# version and Imath release number, e.g. "_Python3_12-3_4"
#

set(PYIMATH_LIB_SUFFIX
    "_Python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}-${IMATH_VERSION_API}"
    CACHE STRING
    "Suffix string for python version and Imath release in library names"
)

set(PYIMATH_OUTPUT_SUBDIR Imath CACHE STRING "Destination sub-folder of the path for install of PyImath headers")

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
