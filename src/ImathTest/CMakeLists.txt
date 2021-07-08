# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

if("${CMAKE_PROJECT_NAME}" STREQUAL "")
  # If the project name is set, this is being configured as a part of
  # Imath.  If there is no project name, it's being configured as a
  # standalone program linking against an already-installed Imath
  # library.
  cmake_minimum_required(VERSION 3.12)
  project(ImathTest)
  find_package(Imath)
endif()

add_executable(ImathTest 
  main.cpp
  testBox.cpp
  testBoxAlgo.cpp
  testColor.cpp
  testExtractEuler.cpp
  testExtractSHRT.cpp
  testFrustum.cpp
  testFrustumTest.cpp
  testFun.cpp
  testInterval.cpp
  testInvert.cpp
  testJacobiEigenSolver.cpp
  testLineAlgo.cpp
  testMatrix.cpp
  testMiscMatrixAlgo.cpp
  testProcrustes.cpp
  testQuat.cpp
  testQuatSetRotation.cpp
  testQuatSlerp.cpp
  testRandom.cpp
  testRoots.cpp
  testShear.cpp
  testTinySVD.cpp
  testVec.cpp
  testArithmetic.cpp
  testBitPatterns.cpp
  testClassification.cpp
  testError.cpp
  testFunction.cpp
  testLimits.cpp
  testSize.cpp
  testToFloat.cpp
  testInterop.cpp
)

target_link_libraries(ImathTest Imath::Imath)
set_target_properties(ImathTest PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )


if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "ImathTest")

  add_executable(ImathHalfCTest half_c_main.c)
  target_link_libraries(ImathHalfCTest Imath::Config)
  target_include_directories(ImathHalfCTest PRIVATE "../Imath")
  #target_compile_options(ImathHalfCTest PRIVATE -mavx2 -mf16c)
  if (IMATH_USE_HALF_LOOKUP_TABLE)
    target_link_libraries(ImathHalfCTest Imath::Imath)
  endif()

  set_target_properties(ImathHalfCTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
  add_test(NAME Imath.half_c_only COMMAND $<TARGET_FILE:ImathHalfCTest>)

endif()

add_executable(ImathHalfPerfTest half_perf_test.cpp)
set_target_properties(ImathHalfPerfTest PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_link_libraries(ImathHalfPerfTest Imath::Imath)

function(DEFINE_IMATH_TESTS)
  foreach(curtest IN LISTS ARGN)
    add_test(NAME Imath.${curtest} COMMAND $<TARGET_FILE:ImathTest> ${curtest})
  endforeach()
endfunction()

define_imath_tests(
  testToFloat
  testSize
  testArithmetic
  testNormalizedConversionError
  testDenormalizedConversionError
  testRoundingError
  testBitPatterns
  testClassification
  testLimits
  testHalfLimits
  testFunction
  testVec
  testColor
  testShear
  testMatrix
  testMiscMatrixAlgo
  testRoots
  testFun
  testInvert
  testInterval
  testFrustum
  testRandom
  testExtractEuler
  testExtractSHRT
  testQuat
  testQuatSetRotation
  testQuatSlerp
  testLineAlgo
  testBoxAlgo
  testBox
  testProcrustes
  testTinySVD
  testJacobiEigenSolver
  testFrustumTest
  testInterop
)

