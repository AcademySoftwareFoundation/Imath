# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#######################################

add_subdirectory(PyImath)
add_subdirectory(PyImathNumpy)

include(CTest)
if(BUILD_TESTING)
  enable_testing()

  add_subdirectory(PyImathTest)
  add_subdirectory(PyImathNumpyTest)
endif()
