# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

find_package(Python COMPONENTS Interpreter REQUIRED)

add_test(PyBindImath 
env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}/../PyBindImath"
${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyBindImathTest)


