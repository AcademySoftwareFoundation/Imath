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
#include <ImathFun.h>
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
    IMATH_INTERNAL_NAMESPACE::C4c testConstructor1i (0);
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

    cout << "rgb2hsv and hsv2rgb" << endl;

    // C4f
    
    IMATH_INTERNAL_NAMESPACE::C4f c4f, r4f, tmp4f;

    for (float r=0.0f; r<=1.0; r += 1.0f)
        for (float g=0.0f; g<=1.0; g += 1.0f)
            for (float b=0.0f; b<=1.0; b += 1.0f)
            {
                c4f.r = r;
                c4f.g = g;
                c4f.b = b;
                c4f.a = 1.0f;
                tmp4f = rgb2hsv(c4f);
                r4f = hsv2rgb(tmp4f);
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4f.r, c4f.r, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4f.g, c4f.g, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4f.b, c4f.b, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4f.a, c4f.a, 1e-5f));
            }
    
    // C3f
    
    IMATH_INTERNAL_NAMESPACE::C3f c3f, r3f, tmp3f;

    for (float r=0.0f; r<=1.0; r += 1.0f)
        for (float g=0.0f; g<=1.0; g += 1.0f)
            for (float b=0.0f; b<=1.0; b += 1.0f)
            {
                c3f.x = r;
                c3f.y = g;
                c3f.z = b;
                tmp3f = rgb2hsv(c3f);
                r3f = hsv2rgb(tmp3f);
                assert (IMATH_INTERNAL_NAMESPACE::equal (r3f.x, c3f.x, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r3f.y, c3f.y, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r3f.z, c3f.z, 1e-5f));
            }
    
    // C4d
    
    IMATH_INTERNAL_NAMESPACE::Color4<double> c4d, r4d, tmp4d;

    for (double r=0.0; r<=1.0; r += 1.0)
        for (double g=0.0; g<=1.0; g += 1.0)
            for (double b=0.0; b<=1.0; b += 1.0)
            {
                c4d.r = r;
                c4d.g = g;
                c4d.b = b;
                c4d.a = 1.0;
                tmp4d = rgb2hsv(c4d);
                r4d = hsv2rgb(tmp4d);
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4d.r, c4d.r, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4d.g, c4d.g, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4d.b, c4d.b, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r4d.a, c4d.a, 1e-5f));
            }
    

    // C3d
    
    IMATH_INTERNAL_NAMESPACE::Color3<double> c3d, r3d, tmp3d;

    for (double r=0.0; r<=1.0; r += 1.0)
        for (double g=0.0; g<=1.0; g += 1.0)
            for (double b=0.0; b<=1.0; b += 1.0)
            {
                c3d.x = r;
                c3d.y = g;
                c3d.z = b;
                tmp3d = rgb2hsv(c3d);
                r3d = hsv2rgb(tmp3d);
                assert (IMATH_INTERNAL_NAMESPACE::equal (r3d.x, c3d.x, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r3d.y, c3d.y, 1e-5f));
                assert (IMATH_INTERNAL_NAMESPACE::equal (r3d.z, c3d.z, 1e-5f));
            }
    
    cout << "ok\n" << endl;
}
