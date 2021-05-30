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
#include <math.h>
#include "testQuatSlerp.h"

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

void
compareQuats (const Quatf& q1, const Quatf& q2, float e)
{
    assert (equalWithAbsError (q1.v.x, q2.v.x, e));
    assert (equalWithAbsError (q1.v.y, q2.v.y, e));
    assert (equalWithAbsError (q1.v.z, q2.v.z, e));
    assert (equalWithAbsError (q1.r, q2.r, e));
}

Quatd
pow (const Quatd q, int n)
{
    Quatd result;

    for (int i = 0; i < n; ++i)
        result *= q;

    return result;
}

void
testSlerp (const Quatf q1, const Quatf q2, int m, int n)
{
    //
    // For two quaternions, q1 and q2, and the identity quaternion, qi,
    //
    //     slerp (q1, q2, f) == q1 * slerp (qi, q1.inverse() * q2, f);  (1)
    //
    // In addition, for integers m and n, with m >= 0, n > 0,
    //
    //     pow (slerp (qi, q3, m/n), n) == pow (q3, m)                  (2)
    //
    // This allows us to test if slerp (q1, q2, m/n) works correctly.
    // Thanks to Dan Piponi for pointing this out.
    //
    // Note that e2, our upper bound for the numerical error in (2) is
    // fairly large.  The reason for this is that testSlerp() will be
    // called with m and n up to 16.  Taking quaternions to the 16th
    // power amplifies any inaccuracies.
    //

    Quatf qi;
    Quatf q3   = q1.inverse() * q2;
    Quatf q1q2 = slerp (q1, q2, float (m) / float (n));
    Quatf qiq3 = slerp (qi, q3, float (m) / float (n));
    float e1   = 60 * std::numeric_limits<float>::epsilon();
    float e2   = 600 * std::numeric_limits<float>::epsilon();

    compareQuats (q1q2, q1 * qiq3, e1);
    compareQuats (pow (qiq3, n), pow (q3, m), e2);
}

void
testSlerp (const Quatf q1, const Quatf q2)
{
    const int n = 16;

    for (int m = 0; m <= n; ++m)
        testSlerp (q1, q2, m, n);
}

void
specificRotations()
{
    cout << "  combinations of 90-degree rotations around x, y and z" << endl;

    for (int x1 = 0; x1 < 3; ++x1)
    {
        V3f axis1 (0, 0, 0);
        axis1[x1] = 1;

        for (int n1 = 0; n1 < 4; ++n1)
        {
            float angle1 = n1 * M_PI / 2;

            Quatf q1;
            q1.setAxisAngle (axis1, angle1);

            for (int x2 = 0; x2 < 3; ++x2)
            {
                V3f axis2 (0, 0, 0);
                axis2[x2] = 1;

                for (int n2 = 0; n2 < 4; ++n2)
                {
                    float angle2 = n2 * M_PI / 2;

                    Quatf q2;
                    q2.setAxisAngle (axis2, angle2);

                    testSlerp (q1, q2);
                    testSlerp (-q1, -q2);

                    if ((q1 ^ q2) < 0.99)
                    {
                        testSlerp (q1, -q2);
                        testSlerp (-q1, q2);
                    }
                }
            }
        }
    }
}

void
randomRotations()
{
    cout << "  random rotations" << endl;

    Rand48 rand (53);

    for (int i = 0; i < 10000; ++i)
    {
        V3f axis1    = hollowSphereRand<V3f> (rand);
        V3f axis2    = hollowSphereRand<V3f> (rand);
        float angle1 = rand.nextf (0, M_PI);
        float angle2 = rand.nextf (0, M_PI);

        Quatf q1, q2;
        q1.setAxisAngle (axis1, angle1);
        q2.setAxisAngle (axis2, angle2);

        testSlerp (q1, q2);
        testSlerp (-q1, -q2);

        if ((q1 ^ q2) < 0.99)
        {
            testSlerp (q1, -q2);
            testSlerp (-q1, q2);
        }
    }
}

} // namespace

void
testQuatSlerp()
{
    cout << "Testing quaternion spherical linear interpolation" << endl;

    specificRotations();
    randomRotations();

    cout << "ok\n" << endl;
}
