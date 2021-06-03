//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathQuat.h>
#include <ImathRandom.h>
#include <assert.h>
#include <iostream>
#include "testQuatSetRotation.h"

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

void
testRotation (const V3f& from, const V3f& to)
{
    //
    // Build a quaternion.
    //

    Quatf Q;
    Q.setRotation (from, to);
    M44f M = Q.toMatrix44();

    //
    // Verify that the quaternion rotates vector from into vector to.
    //

    float e = 20 * std::numeric_limits<float>::epsilon();

    V3f fromM = from * M;
    V3f fromQ = from * Q;
    V3f t0    = to.normalized();
    V3f fM0   = fromM.normalized();
    V3f fQ0   = fromQ.normalized();

    assert (t0.equalWithAbsError (fM0, e));
    assert (t0.equalWithAbsError (fQ0, e));

    //
    // Verify that the rotation axis is the cross product of from and to.
    //

    V3f f0 = from.normalized();

    if (abs (f0 ^ t0) < 0.9)
    {
        V3f n0  = (from % to).normalized();
        V3f n0M = n0 * M;

        assert (n0.equalWithAbsError (n0M, e));
    }
}

void
specificVectors()
{
    cout << "  exact 90-degree rotations" << endl;

    testRotation (V3f (1, 0, 0), V3f (0, 1, 0));
    testRotation (V3f (1, 0, 0), V3f (0, 0, 1));
    testRotation (V3f (0, 1, 0), V3f (1, 0, 0));
    testRotation (V3f (0, 1, 0), V3f (0, 0, 1));
    testRotation (V3f (0, 0, 1), V3f (1, 0, 0));
    testRotation (V3f (0, 0, 1), V3f (0, 1, 0));

    cout << "  exact zero-degree rotations" << endl;

    testRotation (V3f (1, 0, 0), V3f (1, 0, 0));
    testRotation (V3f (0, 1, 0), V3f (0, 1, 0));
    testRotation (V3f (0, 0, 1), V3f (0, 0, 1));
    testRotation (V3f (1, 2, 3), V3f (2, 4, 6));

    cout << "  exact 180-degree rotations" << endl;

    testRotation (V3f (1, 0, 0), V3f (-1, 0, 0));
    testRotation (V3f (0, 1, 0), V3f (0, -1, 0));
    testRotation (V3f (0, 0, 1), V3f (0, 0, -1));
    testRotation (V3f (1, 2, 3), V3f (-2, -4, -6));
    testRotation (V3f (1, 3, 2), V3f (-2, -6, -4));
    testRotation (V3f (2, 1, 3), V3f (-4, -2, -6));
    testRotation (V3f (3, 1, 2), V3f (-6, -2, -4));
    testRotation (V3f (2, 3, 1), V3f (-4, -6, -2));
    testRotation (V3f (3, 2, 1), V3f (-6, -4, -2));

    cout << "  other angles" << endl;

    testRotation (V3f (1, 2, 3), V3f (4, 5, 6));
    testRotation (V3f (1, 2, 3), V3f (4, 6, 5));
    testRotation (V3f (1, 2, 3), V3f (5, 4, 6));
    testRotation (V3f (1, 2, 3), V3f (6, 4, 5));
    testRotation (V3f (1, 2, 3), V3f (5, 6, 4));
    testRotation (V3f (1, 2, 3), V3f (6, 5, 4));
    testRotation (V3f (1, 2, 3), V3f (-4, -5, -6));
    testRotation (V3f (1, 2, 3), V3f (-4, -6, -5));
    testRotation (V3f (1, 2, 3), V3f (-5, -4, -6));
    testRotation (V3f (1, 2, 3), V3f (-6, -4, -5));
    testRotation (V3f (1, 2, 3), V3f (-5, -6, -4));
    testRotation (V3f (1, 2, 3), V3f (-6, -5, -4));
}

void
randomVectors()
{
    cout << "  random from and to vectors" << endl;

    Rand48 rand (17);

    for (int i = 0; i < 500000; ++i)
    {
        V3f from = hollowSphereRand<V3f> (rand) * rand.nextf (0.1, 10.0);
        V3f to   = hollowSphereRand<V3f> (rand) * rand.nextf (0.1, 10.0);
        testRotation (from, to);
    }
}

void
nearlyEqualVectors()
{
    cout << "  nearly equal from and to vectors" << endl;

    Rand48 rand (19);
    float e = 100 * std::numeric_limits<float>::epsilon();

    for (int i = 0; i < 500000; ++i)
    {
        V3f from = hollowSphereRand<V3f> (rand);
        V3f to   = from + e * hollowSphereRand<V3f> (rand);
        testRotation (from, to);
    }
}

void
nearlyOppositeVectors()
{
    cout << "  nearly opposite from and to vectors" << endl;

    Rand48 rand (19);
    float e = 100 * std::numeric_limits<float>::epsilon();

    for (int i = 0; i < 500000; ++i)
    {
        V3f from = hollowSphereRand<V3f> (rand);
        V3f to   = -from + e * hollowSphereRand<V3f> (rand);
        testRotation (from, to);
    }
}

} // namespace

void
testQuatSetRotation()
{
    cout << "Testing quaternion rotations" << endl;

    specificVectors();
    randomVectors();
    nearlyEqualVectors();
    nearlyOppositeVectors();

    cout << "ok\n" << endl;
}
