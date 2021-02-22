# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(GNUInstallDirs)

########################
## Target configuration

# TODO: Right now, we support compiling for multiple pythons at once
set(PYIMATH_OVERRIDE_PYTHON2_INSTALL_DIR "" CACHE STRING "Override the install location for any python 2.x modules compiled")
set(PYIMATH_OVERRIDE_PYTHON3_INSTALL_DIR "" CACHE STRING "Override the install location for any python 3.x modules compiled")

########################
## Build related options

# Suffix to append to root name, this helps with version management
# but can be turned off if you don't care, or otherwise customized
# 
set(PYIMATH_LIB_SUFFIX "-${IMATH_VERSION_API}" CACHE STRING "String added to the end of all the libraries")
# This provides a root for the unique name of the library based on
# the version of python being compiled for
set(PYIMATH_LIB_PYTHONVER_ROOT "_Python" CACHE STRING "String added as a root to the identifier of the python version in the libraries")

########################

# set a default build type if not set
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Code check related features
option(OPENEXR_USE_CLANG_TIDY "Check if clang-tidy is available, and enable that" OFF)
if(OPENEXR_USE_CLANG_TIDY)
  find_program(OPENEXR_CLANG_TIDY_BIN clang-tidy)
  if(OPENEXR_CLANG_TIDY_BIN-NOTFOUND)
    message(FATAL_ERROR "clang-tidy processing requested, but no clang-tidy found")
  endif()
  # TODO: Need to define the list of valid checks and add a file with said list
  set(CMAKE_CXX_CLANG_TIDY
    ${OPENEXR_CLANG_TIDY_BIN};
    -header-filter=.;
    -checks=*;
  )
endif()
