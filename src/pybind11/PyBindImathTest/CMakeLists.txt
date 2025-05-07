# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyBindImathTest")

find_package(Python3 QUIET REQUIRED COMPONENTS Interpreter)

if(NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Cannot find Python3 interpreter")
endif()

set(PYBINDIMATH_MODULE_NAME pybindimath)
set(PYBINDIMATH_MODULE ${PYBINDIMATH_MODULE_NAME}_module)
set(IMATH_LIBRARY Imath)

# Execute the test by invoking python with the script as an
# argument, with PATH and PYTHONPATH set properly

add_test(NAME PyBindImath.PyBindImathTest 
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYBINDIMATH_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${IMATH_LIBRARY}>:$ENV{PATH}"
          ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyBindImathTest.py
)


