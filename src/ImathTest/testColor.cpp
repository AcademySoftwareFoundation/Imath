//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathColor.h>
#include <ImathColorAlgo.h>
#include <ImathMath.h>
#include <assert.h>
#include <iostream>
#include "testColor.h"

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;

void
testColor()
{
    cout << "Testing functions in ImathColor.h & ImathColorAlgo.h" << endl;

    cout << "rgb2packed -> packed2rgb" << endl;

    const float epsilon = std::numeric_limits<float>::epsilon();

    IMATH_INTERNAL_NAMESPACE::PackedColor packed;
    IMATH_INTERNAL_NAMESPACE::C3c in3 (52, 128, 254);
    IMATH_INTERNAL_NAMESPACE::C3c out3;

    packed = IMATH_INTERNAL_NAMESPACE::rgb2packed (in3);
    IMATH_INTERNAL_NAMESPACE::packed2rgb (packed, out3);

    assert (in3 == out3);

    IMATH_INTERNAL_NAMESPACE::C4c testConstructor1;
    IMATH_INTERNAL_NAMESPACE::C4c testConstructor1i (0.f);
    IMATH_INTERNAL_NAMESPACE::C4c testConstructor2 (testConstructor1i);

    testConstructor1 = testConstructor2; // use these so the compiler doesn't emit a warning

    IMATH_INTERNAL_NAMESPACE::C4c testConstructor3 (52, 128, 254, 127);
    IMATH_INTERNAL_NAMESPACE::C4c A (testConstructor3);
    IMATH_INTERNAL_NAMESPACE::C4c B;
    IMATH_INTERNAL_NAMESPACE::C4f X, Y, tmp;

    packed = IMATH_INTERNAL_NAMESPACE::rgb2packed (A);
    IMATH_INTERNAL_NAMESPACE::packed2rgb (packed, B);

    assert (A == B);

    cout << "Imath::Color4 * f" << endl;

    assert ((IMATH_INTERNAL_NAMESPACE::C4f (0.330f, 0.710f, 0.010f, 0.999f) * 0.999f) ==
            IMATH_INTERNAL_NAMESPACE::C4f (0.330f * 0.999f,
                                           0.710f * 0.999f,
                                           0.010f * 0.999f,
                                           0.999f * 0.999f));

    cout << "Imath::Color4 / f" << endl;

    assert ((IMATH_INTERNAL_NAMESPACE::C4f (0.330f, 0.710f, 0.010f, 0.999f) / 0.999f) ==
            IMATH_INTERNAL_NAMESPACE::C4f (0.330f / 0.999f,
                                           0.710f / 0.999f,
                                           0.010f / 0.999f,
                                           0.999f / 0.999f));

    cout << "Assignment and comparison" << endl;

    B = A;
    assert (B == A);
    assert (!(B != A));

    X = Y = IMATH_INTERNAL_NAMESPACE::C4f (0.123f, -0.420f, 0.501f, 0.998f);

    X *= 0.001f;

    assert (std::fabs ((Y.r * 0.001f) - X.r) <= epsilon &&
            std::fabs ((Y.g * 0.001f) - X.g) <= epsilon &&
            std::fabs ((Y.b * 0.001f) - X.b) <= epsilon &&
            std::fabs ((Y.a * 0.001f) - X.a) <= epsilon);

    X = Y = IMATH_INTERNAL_NAMESPACE::C4f (0.123f, -0.420f, 0.501f, 0.998f);

    X /= -1.001f;

    assert (std::fabs ((Y.r / -1.001f) - X.r) <= epsilon &&
            std::fabs ((Y.g / -1.001f) - X.g) <= epsilon &&
            std::fabs ((Y.b / -1.001f) - X.b) <= epsilon &&
            std::fabs ((Y.a / -1.001f) - X.a) <= epsilon);

    Y = IMATH_INTERNAL_NAMESPACE::C4f (0.998f, -0.001f, 0.501f, 1.001f);
    X = IMATH_INTERNAL_NAMESPACE::C4f (0.011f, -0.420f, -0.501f, 0.998f);

    tmp = X + Y;

    assert (std::fabs ((X.r + Y.r) - tmp.r) <= epsilon &&
            std::fabs ((X.g + Y.g) - tmp.g) <= epsilon &&
            std::fabs ((X.b + Y.b) - tmp.b) <= epsilon &&
            std::fabs ((X.a + Y.a) - tmp.a) <= epsilon);

    tmp = X - Y;

    assert (std::fabs ((X.r - Y.r) - tmp.r) <= epsilon &&
            std::fabs ((X.g - Y.g) - tmp.g) <= epsilon &&
            std::fabs ((X.b - Y.b) - tmp.b) <= epsilon &&
            std::fabs ((X.a - Y.a) - tmp.a) <= epsilon);

    tmp = X * Y;

    assert (std::fabs ((X.r * Y.r) - tmp.r) <= epsilon &&
            std::fabs ((X.g * Y.g) - tmp.g) <= epsilon &&
            std::fabs ((X.b * Y.b) - tmp.b) <= epsilon &&
            std::fabs ((X.a * Y.a) - tmp.a) <= epsilon);

    tmp = X / Y;

    //
    // epsilon doesn't work here.
    //
    assert (std::fabs ((X.r / Y.r) - tmp.r) <= 1e-5f &&
            std::fabs ((X.g / Y.g) - tmp.g) <= 1e-5f &&
            std::fabs ((X.b / Y.b) - tmp.b) <= 1e-5f &&
            std::fabs ((X.a / Y.a) - tmp.a) <= 1e-5f);

    tmp = X;
    tmp += Y;

    assert (std::fabs ((X.r + Y.r) - tmp.r) <= epsilon &&
            std::fabs ((X.g + Y.g) - tmp.g) <= epsilon &&
            std::fabs ((X.b + Y.b) - tmp.b) <= epsilon &&
            std::fabs ((X.a + Y.a) - tmp.a) <= epsilon);

    tmp = X;
    tmp -= Y;

    assert (std::fabs ((X.r - Y.r) - tmp.r) <= epsilon &&
            std::fabs ((X.g - Y.g) - tmp.g) <= epsilon &&
            std::fabs ((X.b - Y.b) - tmp.b) <= epsilon &&
            std::fabs ((X.a - Y.a) - tmp.a) <= epsilon);

    tmp = X;
    tmp *= Y;

    assert (std::fabs ((X.r * Y.r) - tmp.r) <= epsilon &&
            std::fabs ((X.g * Y.g) - tmp.g) <= epsilon &&
            std::fabs ((X.b * Y.b) - tmp.b) <= epsilon &&
            std::fabs ((X.a * Y.a) - tmp.a) <= epsilon);

    tmp = X;
    tmp /= Y;

    //
    // epsilon doesn't work here.
    //
    assert (std::fabs ((X.r / Y.r) - tmp.r) <= 1e-5f &&
            std::fabs ((X.g / Y.g) - tmp.g) <= 1e-5f &&
            std::fabs ((X.b / Y.b) - tmp.b) <= 1e-5f &&
            std::fabs ((X.a / Y.a) - tmp.a) <= 1e-5f);

    cout << "ok\n" << endl;
}
