# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.
#
# Patch script run by FetchContent (PATCH_COMMAND) on the internal Boost
# source tree, see src/python/CMakeLists.txt.
#
# Boost.Python's CMakeLists.txt asks for the Python "Development"
# component, which means both "build extension modules" and "embed the
# interpreter". The latter needs libpython, which the manylinux images
# used for building wheels deliberately do not ship, so the request
# fails there. Boost.Python only ever links Python::Module, so narrow
# the request to Development.Module. Idempotent: a second run is a no-op.
#
# Usage: cmake -DBOOST_PYTHON_CMAKELISTS=<path> -P ImathPatchBoostPython.cmake

if(NOT EXISTS "${BOOST_PYTHON_CMAKELISTS}")
  message(FATAL_ERROR "ImathPatchBoostPython: ${BOOST_PYTHON_CMAKELISTS} not found")
endif()

file(READ "${BOOST_PYTHON_CMAKELISTS}" _contents)
string(REPLACE
  "find_package(Python REQUIRED COMPONENTS Development OPTIONAL_COMPONENTS NumPy)"
  "find_package(Python REQUIRED COMPONENTS Development.Module OPTIONAL_COMPONENTS NumPy)"
  _patched "${_contents}")
if(_patched STREQUAL _contents AND NOT _contents MATCHES "COMPONENTS Development\\.Module")
  message(FATAL_ERROR "ImathPatchBoostPython: expected find_package(Python ...) line not found in ${BOOST_PYTHON_CMAKELISTS}; the Boost version may have changed")
endif()
file(WRITE "${BOOST_PYTHON_CMAKELISTS}" "${_patched}")
message(STATUS "ImathPatchBoostPython: Boost.Python now requests Python Development.Module only")
