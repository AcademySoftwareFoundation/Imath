# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

if("${CMAKE_PROJECT_NAME}" STREQUAL "")

  # If the project name is set, this is being configured as a part of
  # Imath.  If there is no project name, it's being configured as a
  # standalone program linking against an already-installed Imath
  # library.

  cmake_minimum_required(VERSION 3.14)
  project(ImathTest)
  find_package(Imath)

endif()

#
# ImathTest
#

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
  testNoInterop.cpp
)

target_link_libraries(ImathTest Imath::Imath)
set_target_properties(ImathTest PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )


if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "ImathTest")

  #
  # ImathHalfCTest
  #

  add_executable(ImathHalfCTest half_c_main.c)
  target_link_libraries(ImathHalfCTest Imath::Imath Imath::Config)

  set_target_properties(ImathHalfCTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

  add_test(NAME ImathHalfCTest COMMAND $<TARGET_FILE:ImathHalfCTest>)

  #
  # ImathHalfPerfTest
  # 

  add_executable(ImathHalfPerfTest half_perf_test.cpp)
  target_link_libraries(ImathHalfPerfTest Imath::Imath)

  set_target_properties(ImathHalfPerfTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

  add_test(NAME ImathHalfPerfTest COMMAND $<TARGET_FILE:ImathHalfPerfTest>)

  set(TESTS
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
    testNoInterop
  )
  
  foreach(curtest IN LISTS TESTS)
    add_test(NAME ImathTest.${curtest} COMMAND $<TARGET_FILE:ImathTest> ${curtest})
  endforeach()
  
endif()

