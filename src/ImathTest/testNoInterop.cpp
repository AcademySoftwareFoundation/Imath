//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#define IMATH_FOREIGN_VECTOR_INTEROP 0

#include <ImathMatrix.h>
#include <ImathVec.h>
#include <cassert>
#include <iostream>

using namespace std;

void
testNoInterop ()
{
    cout << "Testing without interoperability with foreign types" << endl;

    // Construction from a double-indexed array is handled by the
    // interoperability constructors, unless theyre disabled, in which
    // there are fallbacks for the simple types.
    
    {
        const float a[2][2] = {
            { 1.0f, 0.0f },
            { 0.0f, 1.0f }
        };
        IMATH_INTERNAL_NAMESPACE::M22f m(a);
        assert (m[0][0] == a[0][0]);
        assert (m[0][1] == a[0][1]);
        assert (m[1][0] == a[1][0]);
        assert (m[1][1] == a[1][1]);
    }
    
    {
        const float a[3][3] = {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f } 
        };
        IMATH_INTERNAL_NAMESPACE::M33f m(a);
        assert (m[0][0] == a[0][0]);
        assert (m[0][1] == a[0][1]);
        assert (m[0][2] == a[0][2]);
        assert (m[1][0] == a[1][0]);
        assert (m[1][1] == a[1][1]);
        assert (m[1][2] == a[1][2]);
        assert (m[2][0] == a[2][0]);
        assert (m[2][1] == a[2][1]);
        assert (m[2][2] == a[2][2]);
    }
    
    {
        const float a[4][4] = {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f } 
        };
        IMATH_INTERNAL_NAMESPACE::M44f m(a);
        assert (m[0][0] == a[0][0]);
        assert (m[0][1] == a[0][1]);
        assert (m[0][2] == a[0][2]);
        assert (m[1][0] == a[1][0]);
        assert (m[1][1] == a[1][1]);
        assert (m[1][2] == a[1][2]);
        assert (m[2][0] == a[2][0]);
        assert (m[2][1] == a[2][1]);
        assert (m[2][2] == a[2][2]);
    }
    
    cout << "ok\n" << endl;
}
