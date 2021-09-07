# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

# We require this to get object library link library support and
# combined python 2 + 3 support
cmake_minimum_required(VERSION 3.12)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project(PyImath VERSION 3.2.0 LANGUAGES C CXX)

#######################################
#######################################
# This declares all the configuration variables visible
# in cmake-gui or similar and the rest of the global
# project setup
#
# Please look at this file to see what is configurable
#######################################
#######################################
include(config/PyImathSetup.cmake)

# we have a strong dependence on Imath being an exact match
find_package(Imath ${CMAKE_PROJECT_VERSION} REQUIRED CONFIG)

# we are building a python extension, so of course we depend on
# python as well. Except we don't know which version...
# cmake 3.14 can also search for numpy, but we only depend on 3.12
# in the rest of OpenEXR right now...

# first make sure we find *some* python
find_package(Python COMPONENTS Interpreter Development)
if(NOT TARGET Python::Interpreter AND NOT TARGET Python::Python)
  message(WARNING "Unable to find any python interpreter or libraries, disabling PyImath")
  return()
endif()

# we don't need cmake_dependant_option here, because the file is
# included only if -DPYTHON=ON
option(USE_PYTHON2 "Whether to use Python 2.x or 3.x" OFF)

# now determine which (or both), and compile for both
if(USE_PYTHON2)
  find_package(Python2 COMPONENTS Interpreter Development)
  if(Python2_FOUND)
    message(STATUS "Found Python ${Python2_VERSION}")
  elseif(Python2::Python)
    message(WARNING "Found Python ${Python2_VERSION} development libraries, but no interpreter")
  elseif(Python2::Interpreter)
    message(WARNING "Found Python ${Python2_VERSION} interpreter, but no development libraries")
  else()
    message(WARNING "Unable to find Python2 interpreter or development libraries")
  endif()
  set(PY_MAJOR_VERSION ${Python2_VERSION_MAJOR})
  set(Python_VERSION_MAJOR ${Python2_VERSION_MAJOR})
  set(Python_VERSION_MINOR ${Python2_VERSION_MINOR})
  set(Python_EXECUTABLE ${Python2_EXECUTABLE})
  set(Python_SITEARCH ${Python2_SITEARCH})
else()
  find_package(Python3 COMPONENTS Interpreter Development)
  if(Python3_FOUND)
    message(STATUS "Found Python ${Python3_VERSION}")
  elseif(Python3::Python)
    message(WARNING "Found Python ${Python3_VERSION} development libraries, but no interpreter")
  elseif(Python3::Interpreter)
    message(WARNING "Found Python ${Python3_VERSION} interpreter, but no development libraries")
  else()
    message(WARNING "Unable to find Python3 interpreter or development libraries")
  endif()
  set(PY_MAJOR_VERSION ${Python3_VERSION_MAJOR})
  set(Python_VERSION_MAJOR ${Python3_VERSION_MAJOR})
  set(Python_VERSION_MINOR ${Python3_VERSION_MINOR})
  set(Python_EXECUTABLE ${Python3_EXECUTABLE})
  set(Python_SITEARCH ${Python3_SITEARCH})
endif()

if (NOT Python2_FOUND AND NOT Python3_FOUND)
  message(WARNING "Disabling PyImath")
  return()
endif()

# Now that we know what versions of python we have, let's look
# for our other dependency - boost.
# Boost Python has some .. annoyances in that the python module
# has version names attached to it
function(PYIMATH_EXTRACT_REL_SITEARCH varname pyver pyexe pysitearch)
  if(PYIMATH_OVERRIDE_PYTHON_INSTALL_DIR)
    set(${varname} ${PYIMATH_OVERRIDE_PYTHON_INSTALL_DIR} CACHE STRING "Destination sub-folder (relative) for the python ${pyver} modules")
    message(STATUS "Will install to: ${PYIMATH_OVERRIDE_PYTHON_INSTALL_DIR}")
  else()
    get_filename_component(_exedir ${pyexe} DIRECTORY)
    # we do this such that cmake will canonicalize the slashes
    # so the directory search will work under windows and unix
    # consistently
    get_filename_component(_basedir ${pysitearch} DIRECTORY)
    get_filename_component(_basename ${pysitearch} NAME)
    set(_basedir "${_basedir}/${_basename}")
    string(FIND ${_basedir} ${_exedir} _findloc)
    string(LENGTH ${_exedir} _elen)
    while(_findloc EQUAL -1 AND _elen GREATER 0)
      get_filename_component(_nexedir ${_exedir} DIRECTORY)
      string(FIND ${_basedir} ${_nexedir} _findloc)
      if (_nexedir STREQUAL _exedir)
          message(WARNING "Unable to get parent directory for ${_exedir}, using absolute python site arch folder ${pysitearch}")
          set(_elen -1)
          break()
      else()
          set(_exedir ${_nexedir})
      endif()
      string(LENGTH ${_exedir} _elen)
    endwhile()
    math(EXPR _elen "${_elen}+1")
    string(SUBSTRING ${_basedir} ${_elen} -1 _reldir)
    if(APPLE)
      # on macOS, set install path to user's python package directory
      # so that elevated privileges are not necessary
      execute_process(
        COMMAND "${pyexe}" -c "if True:
          import sysconfig
          print(sysconfig.get_path('platlib', 'posix_user'))"
        OUTPUT_VARIABLE _reldir
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()
    set(${varname} ${_reldir} CACHE STRING "Destination sub-folder (relative) for the python ${pyver} modules")
    message(STATUS "Will install to: ${_reldir}")
  endif()
endfunction()

set(PYIMATH_BOOST_PY_COMPONENT "python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")
message(STATUS "Found Python${Python_VERSION_MAJOR} libraries: ${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")
# we can't just use the Python2_SITEARCH variable as that then will
# ignore CMAKE_INSTALL_PREFIX. Could extract this to a function somewhere
# if it is generally useful
pyimath_extract_rel_sitearch(PyImath_Python${Python_VERSION_MAJOR}_SITEARCH_REL ${Python_VERSION_MAJOR} ${Python_EXECUTABLE} ${Python_SITEARCH})

# different flavors of O.S. have multiple versions of python
# some of them have both. Then for boost, some versions of boost
# have just a python component, some it's by major version (python2/python3)
# and still others have maj/min (python27)
# let's run a search and see what we get instead of making it
# an explicit required. The older names are not portable, but
# we'll do the best we can

### NB: We are turning this on globally by default as the boost
###     generated cmake config files seem to be broken and they
###     cross-wire python 2 with 3 in the same variables
option(Boost_NO_BOOST_CMAKE "Disable boost-provided cmake config" ON)
if(Boost_NO_BOOST_CMAKE)
  message(STATUS "Disabling boost-provided cmake config. If this causes problems, consider setting Boost_NO_BOOST_CMAKE variable to OFF")
endif()

find_package(Boost OPTIONAL_COMPONENTS
  python
  python${Python_VERSION_MAJOR}
  ${PYIMATH_BOOST_PY_COMPONENT})
set(_pyimath_have_perver_boost)
if(PYIMATH_BOOST_PY_COMPONENT)
  string(TOUPPER ${PYIMATH_BOOST_PY_COMPONENT} PYIMATH_PY_UPPER)
else()
  set(PYIMATH_BOOST_PY_COMPONENT python${Python_VERSION_MAJOR}x_NOTFOUND)
  set(PYIMATH_PY_UPPER PYTHON${Python_VERSION_MAJOR}X_NOTFOUND)
endif()
if(Boost_PYTHON${Python_VERSION_MAJOR}_FOUND OR Boost_${PYIMATH_PY_UPPER}_FOUND)
  set(_pyimath_have_perver_boost TRUE)
  if(NOT Boost_${PYIMATH_PY_UPPER}_FOUND)
    message(WARNING "Legacy Boost python${Python_VERSION_MAJOR} found, but does not include minor version, this is an old configuration and may not be portable")
    set(PYIMATH_BOOST_PY_COMPONENT python${Python_VERSION_MAJOR})
  endif()
endif()

if(Boost_PYTHON_FOUND AND NOT _pyimath_have_perver_boost)
  # old boost case, I guess we just warn and assume it is python2 (likely)
  message(WARNING "Ambiguous boost python module found, assuming python ${Python_VERSION_MAJOR}. If you have a new boost library, try cleaning the cmake cache and reconfigure with -DBoost_NO_BOOST_CMAKE=ON")
  set(PYIMATH_BOOST_PY_COMPONENT python)
  # set it to a bogus string but not empty so later we don't test against a namespace only target
#  set(PYIMATH_BOOST_PY3_COMPONENT pythonIgnore)
elseif(NOT _pyimath_have_perver_boost)
  message(WARNING "Unable to find boost::python library, disabling PyImath. If you believe this is wrong, check the cmake documentation and see if you need to set Boost_ROOT or Boost_NO_BOOST_CMAKE")
  return()
else()
  if(TARGET Boost::${PYIMATH_BOOST_PY_COMPONENT})
    message(STATUS "Found Python ${Python_VERSION_MAJOR} boost: Boost::${PYIMATH_BOOST_PY_COMPONENT}")
  elseif(Boost_PYTHON_FOUND OR Boost_${PYIMATH_PY_UPPER}_FOUND)
    message(WARNING "Found boost for python ${Python_VERSION_MAJOR}, but FindBoost did not create an import library. If you believe this is wrong, check the cmake documentation and see if you need to set Boost_ROOT or Boost_NO_BOOST_CMAKE")
    return()
  endif()
endif()
unset(PYIMATH_PY_UPPER)
unset(_pyimath_have_perver_boost)

# unfortunately, we can't use the boost numpy stuff, as that requires a
# version of boost that is newer than is mandated by many active versions
# of the VFX reference platform (numpy became active in 1.63 of boost).
# rather than make this an "official" find package thing
include(config/NumPyLocate.cmake)


# utility function for the repeated boilerplate of defining
# the libraries and/or python modules
include(config/ModuleDefine.cmake)

##########################
add_subdirectory(config)

if(NOT BUILD_SHARED_LIBS)
  message(WARNING "Forcing python bindings to be built with dynamic libraries")
endif()

add_subdirectory( PyImath )
if(TARGET Python2::ImathNumPy OR TARGET Python3::ImathNumPy)
  add_subdirectory( PyImathNumpy )
endif()

##########################
# Tests
##########################
include(CTest)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory( PyImathTest )
  add_subdirectory( PyImathSpeedTest )
  if(TARGET Python2::ImathNumPy OR TARGET Python3::ImathNumPy)
    add_subdirectory( PyImathNumpyTest )
  endif()
endif()
