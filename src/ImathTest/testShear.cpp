//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathMath.h>
#include <ImathShear.h>
#include <assert.h>
#include <iostream>
#include "testShear.h"

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;

void
testShear()
{
    cout << "Testing functions in ImathShear.h" << endl;

    cout << "Imath::Shear6 constructors" << endl;

    const float epsilon = std::numeric_limits<float>::epsilon();

    IMATH_INTERNAL_NAMESPACE::Shear6f testConstructor1;
    IMATH_INTERNAL_NAMESPACE::Shear6f testConstructor2 (testConstructor1);

    testConstructor1 = testConstructor2;

    IMATH_INTERNAL_NAMESPACE::Shear6f testConstructor3 (52, 128, 254, 127, 12, -20);
    IMATH_INTERNAL_NAMESPACE::Shear6f A (testConstructor3);
    IMATH_INTERNAL_NAMESPACE::Shear6f B = A;
    IMATH_INTERNAL_NAMESPACE::Shear6f X, Y, tmp;

    assert (A == B);

    cout << "Imath::Shear6 * f" << endl;

    assert ((IMATH_INTERNAL_NAMESPACE::Shear6f (0.330f, 0.710f, 0.010f, 0.999f, -0.531f, -0.012f) *
             0.999f) == IMATH_INTERNAL_NAMESPACE::Shear6f (0.330f * 0.999f,
                                                           0.710f * 0.999f,
                                                           0.010f * 0.999f,
                                                           0.999f * 0.999f,
                                                           -0.531f * 0.999f,
                                                           -0.012f * 0.999f));

    cout << "Imath::Shear6 / f" << endl;

    assert ((IMATH_INTERNAL_NAMESPACE::Shear6f (0.330f, 0.710f, 0.010f, 0.999f, -0.531f, -0.012f) /
             0.999f) == IMATH_INTERNAL_NAMESPACE::Shear6f (0.330f / 0.999f,
                                                           0.710f / 0.999f,
                                                           0.010f / 0.999f,
                                                           0.999f / 0.999f,
                                                           -0.531f / 0.999f,
                                                           -0.012f / 0.999f));

    cout << "Assignment and comparison" << endl;

    B = A;
    assert (B == A);
    assert (!(B != A));

    X = Y = IMATH_INTERNAL_NAMESPACE::Shear6f (0.123f, -0.420f, 0.501f, 0.998f, -0.231f, -0.034f);

    X *= 0.001f;

    assert (std::fabs ((Y.xy * 0.001f) - X.xy) <= epsilon &&
            std::fabs ((Y.xz * 0.001f) - X.xz) <= epsilon &&
            std::fabs ((Y.yz * 0.001f) - X.yz) <= epsilon &&
            std::fabs ((Y.yx * 0.001f) - X.yx) <= epsilon &&
            std::fabs ((Y.zx * 0.001f) - X.zx) <= epsilon &&
            std::fabs ((Y.zy * 0.001f) - X.zy) <= epsilon);

    X = Y = IMATH_INTERNAL_NAMESPACE::Shear6f (0.123f, -0.420f, 0.501f, 0.998f, -0.231f, -0.034f);

    X /= -1.001f;

    assert (std::fabs ((Y.xy / -1.001f) - X.xy) <= epsilon &&
            std::fabs ((Y.xz / -1.001f) - X.xz) <= epsilon &&
            std::fabs ((Y.yz / -1.001f) - X.yz) <= epsilon &&
            std::fabs ((Y.yx / -1.001f) - X.yx) <= epsilon &&
            std::fabs ((Y.zx / -1.001f) - X.zx) <= epsilon &&
            std::fabs ((Y.zy / -1.001f) - X.zy) <= epsilon);

    Y = IMATH_INTERNAL_NAMESPACE::Shear6f (0.998f, -0.001f, 0.501f, 1.001f, -0.231f, -0.034f);
    X = IMATH_INTERNAL_NAMESPACE::Shear6f (0.011f, -0.420f, -0.501f, 0.998f, -0.231f, -0.034f);

    tmp = X + Y;

    assert (std::fabs ((X.xy + Y.xy) - tmp.xy) <= epsilon &&
            std::fabs ((X.xz + Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz + Y.yz) - tmp.yz) <= epsilon &&
            std::fabs ((X.yx + Y.yx) - tmp.yx) <= epsilon &&
            std::fabs ((X.zx + Y.zx) - tmp.zx) <= epsilon &&
            std::fabs ((X.zy + Y.zy) - tmp.zy) <= epsilon);

    tmp = X - Y;

    assert (std::fabs ((X.xy - Y.xy) - tmp.xy) <= epsilon &&
            std::fabs ((X.xz - Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz - Y.yz) - tmp.yz) <= epsilon &&
            std::fabs ((X.yx - Y.yx) - tmp.yx) <= epsilon &&
            std::fabs ((X.zx - Y.zx) - tmp.zx) <= epsilon &&
            std::fabs ((X.zy - Y.zy) - tmp.zy) <= epsilon);

    tmp = X * Y;

    assert (std::fabs ((X.xy * Y.xy) - tmp.xy) <= epsilon &&
            std::fabs ((X.xz * Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz * Y.yz) - tmp.yz) <= epsilon &&
            std::fabs ((X.yx * Y.yx) - tmp.yx) <= epsilon &&
            std::fabs ((X.zx * Y.zx) - tmp.zx) <= epsilon &&
            std::fabs ((X.zy * Y.zy) - tmp.zy) <= epsilon);

    tmp = X / Y;

    //
    // epsilon doesn't work here.
    //
    assert (std::fabs ((X.xy / Y.xy) - tmp.xy) <= 1e-5f &&
            std::fabs ((X.xz / Y.xz) - tmp.xz) <= 1e-5f &&
            std::fabs ((X.yz / Y.yz) - tmp.yz) <= 1e-5f &&
            std::fabs ((X.yx / Y.yx) - tmp.yx) <= 1e-5f &&
            std::fabs ((X.zx / Y.zx) - tmp.zx) <= 1e-5f &&
            std::fabs ((X.zy / Y.zy) - tmp.zy) <= 1e-5f);

    tmp = X;
    tmp += Y;

    assert (std::fabs ((X.xy + Y.xy) - tmp.xy) <= epsilon &&
            std::fabs ((X.xz + Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz + Y.yz) - tmp.yz) <= epsilon &&
            std::fabs ((X.yx + Y.yx) - tmp.yx) <= epsilon &&
            std::fabs ((X.zx + Y.zx) - tmp.zx) <= epsilon &&
            std::fabs ((X.zy + Y.zy) - tmp.zy) <= epsilon);

    tmp = X;
    tmp -= Y;

    assert (std::fabs ((X.xy - Y.xy) - tmp.xy) <= epsilon &&
            std::fabs ((X.xz - Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz - Y.yz) - tmp.yz) <= epsilon &&
            std::fabs ((X.yx - Y.yx) - tmp.yx) <= epsilon &&
            std::fabs ((X.xz - Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz - Y.yz) - tmp.yz) <= epsilon);

    tmp = X;
    tmp *= Y;

    assert (std::fabs ((X.xy * Y.xy) - tmp.xy) <= epsilon &&
            std::fabs ((X.xz * Y.xz) - tmp.xz) <= epsilon &&
            std::fabs ((X.yz * Y.yz) - tmp.yz) <= epsilon &&
            std::fabs ((X.yx * Y.yx) - tmp.yx) <= epsilon &&
            std::fabs ((X.zx * Y.zx) - tmp.zx) <= epsilon &&
            std::fabs ((X.zy * Y.zy) - tmp.zy) <= epsilon);

    tmp = X;
    tmp /= Y;

    //
    // epsilon doesn't work here.
    //
    assert (std::fabs ((X.xy / Y.xy) - tmp.xy) <= 1e-5f &&
            std::fabs ((X.xz / Y.xz) - tmp.xz) <= 1e-5f &&
            std::fabs ((X.yz / Y.yz) - tmp.yz) <= 1e-5f &&
            std::fabs ((X.yx / Y.yx) - tmp.yx) <= 1e-5f &&
            std::fabs ((X.zx / Y.zx) - tmp.zx) <= 1e-5f &&
            std::fabs ((X.zy / Y.zy) - tmp.zy) <= 1e-5f);

    cout << "ok\n" << endl;
}
