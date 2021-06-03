//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "testArithmetic.h"
#include "testBitPatterns.h"
#include "testClassification.h"
#include "testError.h"
#include "testFunction.h"
#include "testLimits.h"
#include "testSize.h"
#include "testToFloat.h"
#include "testBox.h"
#include "testBoxAlgo.h"
#include "testColor.h"
#include "testExtractEuler.h"
#include "testExtractSHRT.h"
#include "testFrustum.h"
#include "testFrustumTest.h"
#include "testFun.h"
#include "testInterop.h"
#include "testInterval.h"
#include "testInvert.h"
#include "testJacobiEigenSolver.h"
#include "testLineAlgo.h"
#include "testMatrix.h"
#include "testMiscMatrixAlgo.h"
#include "testProcrustes.h"
#include "testQuat.h"
#include "testQuatSetRotation.h"
#include "testQuatSlerp.h"
#include "testRandom.h"
#include "testRoots.h"
#include "testShear.h"
#include "testTinySVD.h"
#include "testVec.h"

#include <iostream>
#include <string.h>

#define TEST(x)                                                                                    \
    if (argc < 2 || !strcmp (argv[1], #x))                                                         \
        x();

int
main (int argc, char* argv[])
{
    // NB: If you add a test here, make sure to enumerate it in the
    // CMakeLists.txt so it runs as part of the test suite
    TEST (testToFloat);
    TEST (testSize);
    TEST (testArithmetic);
    TEST (testNormalizedConversionError);
    TEST (testDenormalizedConversionError);
    TEST (testRoundingError);
    TEST (testBitPatterns);
    TEST (testClassification);
    TEST (testLimits);
    TEST (testHalfLimits);
    TEST (testFunction);
    TEST (testVec);
    TEST (testColor);
    TEST (testShear);
    TEST (testMatrix);
    TEST (testMiscMatrixAlgo);
    TEST (testRoots);
    TEST (testFun);
    TEST (testInvert);
    TEST (testInterval);
    TEST (testFrustum);
    TEST (testRandom);
    TEST (testExtractEuler);
    TEST (testExtractSHRT);
    TEST (testQuat);
    TEST (testQuatSetRotation);
    TEST (testQuatSlerp);
    TEST (testLineAlgo);
    TEST (testBoxAlgo);
    TEST (testBox);
    TEST (testProcrustes);
    TEST (testTinySVD);
    TEST (testJacobiEigenSolver);
    TEST (testFrustumTest);
    TEST (testInterop);
    // NB: If you add a test here, make sure to enumerate it in the
    // CMakeLists.txt so it runs as part of the test suite

    return 0;
}
