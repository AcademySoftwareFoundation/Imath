# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# This test has two parts:
#   1. The PyImathTestC executable, which validates the C++ API from the
#      PyImath library
#   2. The pyImathTest.py python code that validates the behavior of
#      the wrappings
#

if("${CMAKE_PROJECT_NAME}" STREQUAL "")

  #
  # This allows the PyImathTest project to serve as an example of a
  # project using Imath as an external dependency, used in the CI workflow
  # to validate that the project exports the proper CMake config.
  #
  # Treat the test source and executable as a project relying on Imath
  # as a pre-built library.
  #
  # This section is activated when running cmake with
  # src/python/PyImathTest as the source directory (with
  # CMAKE_PREFIX_PATH set to an imath installation)
  #

  message(STATUS "Building PyImathTest as standalone project")

  cmake_minimum_required(VERSION 3.12)
  project(PyImathTest)

  find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
  find_package(Boost REQUIRED COMPONENTS python)

  find_package(Imath COMPONENTS PyImath)
endif()

message (STATUS "Configuring PyImathTest")

set(PYIMATH_MODULE_NAME imath) 
set(PYIMATH_MODULE ${PYIMATH_MODULE_NAME}_module)
set(PYIMATH_LIBRARY PyImath)

set(PYIMATH_TEST_SOURCES main.cpp testStringTable.cpp)

add_executable(PyImathTestC ${PYIMATH_TEST_SOURCES})

target_link_libraries(PyImathTestC 
    Imath::Imath
    Imath::PyImath
    Boost::boost
    Boost::python
    Python3::Python
)

# Test of the exported PyImath C++ API

add_test(NAME PyImath.PyImathTestC COMMAND $<TARGET_FILE:PyImathTestC>)
set_tests_properties(PyImath.PyImathTestC PROPERTIES ENVIRONMENT PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>)

target_include_directories(PyImathTestC PUBLIC ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

set_target_properties(PyImathTestC PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Test of the python API.  Execute the test by invoking python
# with the script as an argument, with PATH and PYTHONPATH set properly.
  
add_test(NAME PyImath.PyImathTest
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>:$ENV{PATH}"
          ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.py
)

#
# disable the self-assig warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYIMATH_TEST_SOURCES} 
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_source_files_properties(
    ${PYIMATH_TEST_SOURCES} 
    PROPERTIES COMPILE_FLAGS "-Wdeprecated-declarations"
  )
endif()


  

  
