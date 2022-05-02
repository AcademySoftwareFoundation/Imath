//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "testInvert.h"
#include <ImathMatrix.h>
#include <ImathMatrixAlgo.h>
#include <assert.h>
#include <iostream>

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

void
invertM44f (const M44f& m, float e)
{
    M44f inv1   = m.inverse ();
    M44f inv2   = m.gjInverse ();
    M44f ident1 = m * inv1;
    M44f ident2 = m * inv2;

    //cout << "m\n" << m << endl;
    //cout << "inv1\n" << inv1 << "ident1\n" << ident1 << endl;
    //cout << "inv2\n" << inv2 << "ident2\n" << ident2 << endl;

    assert (ident1.equalWithAbsError (identity44f, e));
    assert (ident2.equalWithAbsError (identity44f, e));
}

void
invertM33f (const M33f& m, float e)
{
    M33f inv1   = m.inverse ();
    M33f inv2   = m.gjInverse ();
    M33f ident1 = m * inv1;
    M33f ident2 = m * inv2;

    //cout << "m\n" << m << endl;
    //cout << "inv1\n" << inv1 << "ident1\n" << ident1 << endl;
    //cout << "inv2\n" << inv2 << "ident2\n" << ident2 << endl;

    assert (ident1.equalWithAbsError (identity33f, e));
    assert (ident2.equalWithAbsError (identity33f, e));
}

} // namespace

void
testInvert ()
{
    cout << "Testing 4x4 and 3x3 matrix inversion:" << endl;

    {
        cout << "M44f" << endl;

        // clang-format off

	M44f m1 ( 1,  0,  0,  0,
		  0,  1,  0,  0,
		  0,  0,  1,  0,
		  0,  0,  0,  1);

	M44f m2 ( 0,  1,  0,  0,
		 -1,  0,  0,  0,
		  0,  0,  1,  0,
		  0,  0,  0,  1);

	M44f m3 ( 1,  0,  0,  0,
		  0,  2,  0,  0,
		  0,  0,  0, -1,
		  0,  0,  1,  0);

	M44f m4 ( 4.683281e-01f, -8.749647e-01f,  1.229049e-01f,  0.000000e+00f,
		  1.251189e-02f,  1.456563e-01f,  9.892561e-01f,  0.000000e+00f,
		 -8.834660e-01f, -4.617587e-01f,  7.916244e-02f,  0.000000e+00f,
		 -4.726541e+00f,  3.044795e+00f, -6.737138e+00f,  1.000000e+00f);

	M44f m5 ( 4.683281e-01f, -8.749647e-01f,  1.229049e-01f,  1.000000e+00f,
		  1.251189e-02f,  1.456563e-01f,  9.892561e-01f,  2.000000e+00f,
		 -8.834660e-01f, -4.617587e-01f,  7.916244e-02f,  3.000000e+00f,
		 -4.726541e+00f,  3.044795e+00f, -6.737138e+00f,  4.000000e+00f);

        // clang-format on

        invertM44f (m1, 0);
        invertM44f (m2, 0);
        invertM44f (m3, 0);
        invertM44f (m4, 1e-6f);
        invertM44f (m5, 1e-6f);
    }

    {
        cout << "M33f" << endl;

        // clang-format off

	M33f m1 ( 1,  0,  0,
		  0,  1,  0,
		  0,  0,  1);

	M33f m2 ( 0,  1,  0,
		 -1,  0,  0,
		  0,  0,  1);

	M33f m3 ( 2,  0,  0,
		  0,  0, -1,
		  0,  1,  0);

	M33f m4 ( 4.683281e-01f, -8.749647e-01f,  0.000000e+00f,
		  1.251189e-02f,  1.456563e-01f,  0.000000e+00f,
		  0.000000e+00f,  0.000000e+00f,  1.000000e+00f);

	M33f m5 ( 4.683281e-01f, -8.749647e-01f,  1.229049e-01f,
		  1.251189e-02f,  1.456563e-01f,  9.892561e-01f,
		 -8.834660e-01f, -4.617587e-01f,  7.916244e-02f);

        // clang-format on

        invertM33f (m1, 0);
        invertM33f (m2, 0);
        invertM33f (m3, 0);
        invertM33f (m4, 1e-6f);
        invertM33f (m5, 1e-6f);
    }

    cout << "ok\n" << endl;
}
