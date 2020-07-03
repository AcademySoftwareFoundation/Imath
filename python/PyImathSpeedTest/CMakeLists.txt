# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

# Speed tests are not performed in Python2 as getting acurate time is difficult.
# set -DSPEED=ON when running cmake to perform these tests.
if(TARGET Python3::Interpreter AND SPEED) 
  add_test(PyImath.PyImathSpeedTest_Python3
    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathSpeedTest.in
    )
  set_tests_properties(PyImath.PyImathSpeedTest_Python3 PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
    )
endif()
