# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyImathNumpyTest")

set(PYIMATHNUMPY_MODULE imathnumpy)
set(PYIMATH_LIBRARY PyImath)

# Execute the test by invoking python with the script as an
# argument, with PATH and PYTHONPATH set properly

add_test(NAME PyImath.PyImathNumpyTest
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATHNUMPY_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>:$ENV{PATH}"
          ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathNumpyTest.py
)
