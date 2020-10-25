# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

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
)

target_link_libraries(ImathTest Imath::Imath)
set_target_properties(ImathTest PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
add_test(NAME Imath COMMAND $<TARGET_FILE:ImathTest>)

