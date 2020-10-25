# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

add_executable(CImathTest
  main.cpp
  testHalf.cpp
)

target_link_libraries(CImathTest CImath Imath)
set_target_properties(CImathTest PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
add_test(NAME CImath COMMAND $<TARGET_FILE:CImathTest>)
