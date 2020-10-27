# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

add_executable(CImathTest_Cpp
  testCImath.cpp
)

target_link_libraries(CImathTest_Cpp CImath Imath)
set_target_properties(CImathTest_Cpp PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
add_test(NAME CImath_Cpp COMMAND $<TARGET_FILE:CImathTest_Cpp>)

project(CImathTest_C C)

add_executable(CImathTest_C
  testCImath.c
)

target_link_libraries(CImathTest_C CImath Imath)
set_target_properties(CImathTest_C PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
add_test(NAME CImath_C COMMAND $<TARGET_FILE:CImathTest_C>)
set_property(TARGET CImathTest_C PROPERTY C_STANDARD 99)
