# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message(STATUS "Configuring imathnumpy module")

#
# Python module, identified by PYIMATHNUMPY_MODULE
#

set(PYIMATHNUMPY_MODULE imathnumpy)

set(PYIMATHNUMPY_MODULE_SOURCES imathnumpymodule.cpp)

# Build "imathnumpy", not "imathnumpy_d" in Debug.
# Note this must appear before add_library

set(CMAKE_DEBUG_POSTFIX "") 

add_library(${PYIMATHNUMPY_MODULE} MODULE ${PYIMATHNUMPY_MODULE_SOURCES})

# Ensure correct module extension on Windows
if(WIN32)
    set_target_properties(${PYIMATHNUMPY_MODULE} PROPERTIES SUFFIX ".pyd")
endif()

set_target_properties(${PYIMATHNUMPY_MODULE} PROPERTIES
    PREFIX ""
    OUTPUT_NAME ${PYIMATHNUMPY_MODULE} # override the _d suffix for Debug builds
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
)

# Ask python for the numpy include dir
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(${PYIMATHNUMPY_MODULE}
  PUBLIC ${NUMPY_INCLUDE_DIR} ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

target_link_libraries(${PYIMATHNUMPY_MODULE}
    PRIVATE
        Imath::Config
        PyImath
        Boost::boost
        Boost::python
        Python3::Module
)

if (IMATH_INSTALL)

  #
  # Set the PYTHON_INSTALL_DIR but only if it's not already set,
  # which allows an externally-set value to take effect.
  #
  
  if (NOT DEFINED PYTHON_INSTALL_DIR)

    set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
    message(STATUS "installing ${PYIMATHNUMPY_MODULE} module to ${PYTHON_INSTALL_DIR}")

  else()

    message(STATUS "installing ${PYIMATHNUMPY_MODULE} module to ${PYTHON_INSTALL_DIR} (set externally)")
  
  endif()
  
  install(TARGETS ${PYIMATHNUMPY_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

endif()

#
# disable the self-assig warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    imathnumpymodule.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_source_files_properties(
    imathnumpymodule.cpp
    PROPERTIES COMPILE_FLAGS "-Wdeprecated-declarations"
  )
endif()

target_compile_definitions(${PYIMATHNUMPY_MODULE} PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)
