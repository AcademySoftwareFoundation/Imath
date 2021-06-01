//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathBox.h>
#include <ImathFrustum.h>
#include <ImathFrustumTest.h>
#include <ImathSphere.h>
#include <assert.h>
#include <iostream>
#include "testFrustumTest.h"

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;

void
testFrustumTest()
{
    cout << "Testing functions in ImathFrustumTest.h";

    cout << "\nisVisible(Vec3) ";

    float n = 1.7;
    float f = 567.0;
    float l = -3.5;
    float r = 2.0;
    float b = -1.3;
    float t = 0.9;

    IMATH_INTERNAL_NAMESPACE::Frustum<float> frustum (n, f, l, r, t, b, false);

    IMATH_INTERNAL_NAMESPACE::Matrix44<float> cameraMat;
    IMATH_INTERNAL_NAMESPACE::Vec3<float> cameraPos (100.0f, 200.0f, 300.0f);
    cameraMat.makeIdentity();
    cameraMat.translate (cameraPos);

    IMATH_INTERNAL_NAMESPACE::FrustumTest<float> frustumTest (frustum, cameraMat);

    /////////////////////////////////////////////////////
    // Test Vec3's
    IMATH_INTERNAL_NAMESPACE::Vec3<float> insideVec (100.0f, 200.0f, 300 - 2.0f);
    IMATH_INTERNAL_NAMESPACE::Vec3<float> outsideVec_near (100.0f, 200.0f, 300 - 1.5f);
    IMATH_INTERNAL_NAMESPACE::Vec3<float> outsideVec_far (100.0f, 200.0f, 300 - 568.0f);
    IMATH_INTERNAL_NAMESPACE::Vec3<float> outsideVec_side (100.0f, 200.0f + 100.0f, 300 - 2.0f);
    IMATH_INTERNAL_NAMESPACE::Vec3<float> outsideVec_up (100.0f + 100.0f, 200.0f, 300 - 2.0f);

    assert (frustumTest.isVisible (insideVec));
    assert (!frustumTest.isVisible (outsideVec_near));
    assert (!frustumTest.isVisible (outsideVec_far));
    assert (!frustumTest.isVisible (outsideVec_side));
    assert (!frustumTest.isVisible (outsideVec_up));
    cout << "passed Vec3\n";

    /////////////////////////////////////////////////////
    // Test Boxes
    IMATH_INTERNAL_NAMESPACE::Vec3<float> tinySize (0.0001f, 0.0001f, 0.0001f);
    IMATH_INTERNAL_NAMESPACE::Vec3<float> hugeSize (1000.0f, 1000.0f, 1000.0f);

    // Empty box should NOT be visible
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>>()));

    // Tiny box inside the frust should be visible
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            insideVec + tinySize,
            insideVec + tinySize)));

    // Huge boxes inside and outside should be visible
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            insideVec - hugeSize,
            insideVec + hugeSize)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_near - hugeSize,
            outsideVec_near + hugeSize)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_far - hugeSize,
            outsideVec_far + hugeSize)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_side - hugeSize,
            outsideVec_side + hugeSize)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_up - hugeSize,
            outsideVec_up + hugeSize)));

    // Tiny boxes outside should NOT be visible
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_near - tinySize,
            outsideVec_near + tinySize)));
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_far - tinySize,
            outsideVec_far + tinySize)));
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_side - tinySize,
            outsideVec_side + tinySize)));
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Box<IMATH_INTERNAL_NAMESPACE::Vec3<float>> (
            outsideVec_up - tinySize,
            outsideVec_up + tinySize)));
    cout << "passed Box\n";

    /////////////////////////////////////////////////////
    // Test Spheres
    float tinyRadius = 0.0001f;
    float hugeRadius = 1000.0f;

    // Tiny sphere inside the frust should be visible
    assert (
        frustumTest.isVisible (IMATH_INTERNAL_NAMESPACE::Sphere3<float> (insideVec, tinyRadius)));

    // Huge spheres inside and outside should be visible
    assert (
        frustumTest.isVisible (IMATH_INTERNAL_NAMESPACE::Sphere3<float> (insideVec, hugeRadius)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_near, hugeRadius)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_far, hugeRadius)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_side, hugeRadius)));
    assert (frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_up, hugeRadius)));

    // Tiny spheres outside should NOT be visible
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_near, tinyRadius)));
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_far, tinyRadius)));
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_side, tinyRadius)));
    assert (!frustumTest.isVisible (
        IMATH_INTERNAL_NAMESPACE::Sphere3<float> (outsideVec_up, tinyRadius)));
    cout << "passed Sphere\n";

    cout << "\nok\n\n";
}
