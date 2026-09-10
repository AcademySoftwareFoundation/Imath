# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(GNUInstallDirs)

#
# Python and Boost
#

# Development.Module is all the extension modules need. Development.Embed
# (libpython) is only needed by the PyImathTestC test program and is not
# available in the manylinux images used to build wheels. The split
# components exist in CMake 3.18+; older CMake only knows Development.
if(CMAKE_VERSION VERSION_LESS 3.18)
  find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
  set(PYIMATH_PYTHON_EMBED_FOUND ${Python3_Development_FOUND})
else()
  find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module OPTIONAL_COMPONENTS Development.Embed)
  set(PYIMATH_PYTHON_EMBED_FOUND ${Python3_Development.Embed_FOUND})
endif()

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Could not find Python")
endif()

#
# Find or build Boost.Python.
#
# By default, use an installed Boost. If IMATH_FORCE_INTERNAL_BOOST is
# ON, or if no installed Boost with the python component can be found
# (e.g. inside a bare cibuildwheel container), download the pinned
# Boost release and build a private, static Boost.Python as part of
# this build, against the same interpreter found by find_package(Python3)
# above. This mirrors OPENEXR_FORCE_INTERNAL_IMATH in OpenEXR.
#

option(IMATH_FORCE_INTERNAL_BOOST "Force building an internal, static Boost.Python instead of using an installed Boost" OFF)
set(IMATH_BOOST_URL "https://github.com/boostorg/boost/releases/download/boost-1.92.0/boost-1.92.0-cmake.tar.xz" CACHE STRING "URL of the Boost CMake release tarball for auto-build of Boost.Python")
set(IMATH_BOOST_URL_HASH "SHA256=9bed76128d4e46755dbe818487788c6fceb6f72b378f4daa49b7e1e600d9088d" CACHE STRING "Expected hash of IMATH_BOOST_URL (empty to skip verification)")

if(NOT IMATH_FORCE_INTERNAL_BOOST)
  find_package(Boost CONFIG QUIET COMPONENTS python)
endif()

if(Boost_FOUND)

  message(STATUS "Using Boost ${Boost_VERSION} from ${Boost_DIR}")
  set(IMATH_USE_INTERNAL_BOOST OFF)

else()

  if(IMATH_FORCE_INTERNAL_BOOST)
    message(STATUS "Boost forced internal, fetching from ${IMATH_BOOST_URL}")
  else()
    message(STATUS "Boost with python component was not found, fetching from ${IMATH_BOOST_URL}")
  endif()
  set(IMATH_USE_INTERNAL_BOOST ON)

  if(CMAKE_VERSION VERSION_LESS 3.18)
    message(FATAL_ERROR "Building the internal Boost.Python requires CMake 3.18 or newer (have ${CMAKE_VERSION})")
  endif()

  # Boost's own CMake uses find_package(Python), while Imath uses
  # find_package(Python3). Seed the former with the interpreter found
  # by the latter so both agree on the same Python.
  set(Python_EXECUTABLE "${Python3_EXECUTABLE}")
  find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)

  # Only the libraries PyImath uses, plus their dependencies, are
  # configured. Boost::boost is provided by the compatibility targets.
  set(BOOST_ENABLE_PYTHON ON)
  set(BOOST_ENABLE_COMPATIBILITY_TARGETS ON)
  set(BOOST_INCLUDE_LIBRARIES python any format multi_index)

  # Build Boost.Python static and position-independent so it links
  # directly into the python modules, without affecting how Imath and
  # PyImath themselves are built.
  set(_imath_saved_build_shared_libs ${BUILD_SHARED_LIBS})
  set(_imath_saved_pic ${CMAKE_POSITION_INDEPENDENT_CODE})
  set(BUILD_SHARED_LIBS OFF)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)

  include(FetchContent)
  if(IMATH_BOOST_URL_HASH)
    set(_imath_boost_url_hash URL_HASH "${IMATH_BOOST_URL_HASH}")
  else()
    set(_imath_boost_url_hash)
  endif()
  # Narrow Boost.Python's Python component request, see the patch script.
  set(_imath_boost_patch PATCH_COMMAND ${CMAKE_COMMAND}
      -DBOOST_PYTHON_CMAKELISTS=<SOURCE_DIR>/libs/python/CMakeLists.txt
      -P ${PROJECT_SOURCE_DIR}/cmake/ImathPatchBoostPython.cmake)
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    FetchContent_Declare(Boost URL "${IMATH_BOOST_URL}" ${_imath_boost_url_hash} ${_imath_boost_patch} DOWNLOAD_EXTRACT_TIMESTAMP TRUE EXCLUDE_FROM_ALL)
    FetchContent_MakeAvailable(Boost)
  else()
    FetchContent_Declare(Boost URL "${IMATH_BOOST_URL}" ${_imath_boost_url_hash} ${_imath_boost_patch} DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_GetProperties(Boost)
    if(NOT boost_POPULATED)
      FetchContent_Populate(Boost)
      add_subdirectory(${boost_SOURCE_DIR} ${boost_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
  endif()

  set(BUILD_SHARED_LIBS ${_imath_saved_build_shared_libs})
  set(CMAKE_POSITION_INDEPENDENT_CODE ${_imath_saved_pic})
  unset(_imath_saved_build_shared_libs)
  unset(_imath_saved_pic)
  unset(_imath_boost_url_hash)
  unset(_imath_boost_patch)

endif()

if(NOT TARGET Boost::python)
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

  # Ensure Python3_SITEARCH is an absolute path
  get_filename_component(Python3_SITEARCH_ABS "${Python3_SITEARCH}" REALPATH)

  file(RELATIVE_PATH PYTHON_INSTALL_DIR
    "${Python3_PREFIX}"
    "${Python3_SITEARCH_ABS}")

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
# Python extension suffix.
# In cmake 3.17+ `Python_add_library` will handle this automatically
# and then this code can be removed.
#

# Example suffix is '.cp311-win_amd64.pyd'.
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))"
    OUTPUT_VARIABLE PYIMATH_EXTENSION_SUFFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

#
# PyImath and PyImathNumpy
#

add_subdirectory( PyImath )

option(PYIMATHNUMPY "Set ON to compile the imathnumpy module" ON)
if (PYIMATHNUMPY)
  add_subdirectory( PyImathNumpy )
endif()

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
  if (PYIMATHNUMPY)
    add_subdirectory( PyImathNumpyTest )
  endif()
endif()
