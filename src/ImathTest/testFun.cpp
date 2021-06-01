//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathFun.h>
#if __cplusplus >= 202002L
#    include <bit>
#endif
#include <iostream>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include "testFun.h"

using namespace std;

#if ULONG_MAX == 18446744073709551615LU
typedef long unsigned int Int64;
#else
    typedef long long unsigned int Int64;
#endif

#if __cplusplus < 202002L
    template <typename To, typename From>
    static inline To
    bit_cast (From from)
    {
        static_assert (sizeof (From) == sizeof (To), "Type sizes do not match");
        union
        {
            From f;
            To   t;
        } u;
        u.f = from;
        return u.t;
    }
#endif

void
testf (float f, bool changeExpected = true)
{
    printf ("\n");

    float sf = IMATH_INTERNAL_NAMESPACE::succf (f);
    float pf = IMATH_INTERNAL_NAMESPACE::predf (f);
    float spf = IMATH_INTERNAL_NAMESPACE::succf (IMATH_INTERNAL_NAMESPACE::predf (f));
    float psf = IMATH_INTERNAL_NAMESPACE::predf (IMATH_INTERNAL_NAMESPACE::succf (f));

    printf ("f %.9g\n", f);
    printf ("sf %.9g\n", sf);
    printf ("pf %.9g\n", pf);
    printf ("spf %.9g\n", spf);
    printf ("psf %.9g\n", psf);

    fflush (stdout);

    if (changeExpected)
    {
        assert (pf < f);
        assert (f < sf);
    }
    else
    {
        // No bit change expected if input was inf or NaN
        assert (bit_cast<unsigned> (pf) == bit_cast<unsigned> (f));
        assert (bit_cast<unsigned> (sf) == bit_cast<unsigned> (f));
    }
}

void
testd (double d, bool changeExpected = true)
{
    printf ("\n");

    double sd = IMATH_INTERNAL_NAMESPACE::succd (d);
    double pd = IMATH_INTERNAL_NAMESPACE::predd (d);
    double spd = IMATH_INTERNAL_NAMESPACE::succd (IMATH_INTERNAL_NAMESPACE::predd (d));
    double psd = IMATH_INTERNAL_NAMESPACE::predd (IMATH_INTERNAL_NAMESPACE::succd (d));

    printf ("d %.18lg\n", d);
    printf ("sd %.18lg\n", sd);
    printf ("pd %.18lg\n", pd);
    printf ("spd %.18lg\n", spd);
    printf ("psd %.18lg\n", psd);

    fflush (stdout);

    if (changeExpected)
    {
        assert (pd < d);
        assert (d < sd);
    }
    else
    {
        // No bit change expected if input was inf or NaN
        assert (bit_cast<Int64> (pd) == bit_cast<Int64> (d));
        assert (bit_cast<Int64> (sd) == bit_cast<Int64> (d));
    }
}

void
testFun()
{
    cout << "Testing functions in ImathFun.h" << endl;

    cout << "floor" << endl;

    assert (IMATH_INTERNAL_NAMESPACE::floor (0.0f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::floor (0.5f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::floor (-0.5f) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::floor (1.0f) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::floor (-1.0f) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::floor (1.5f) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::floor (-1.5f) == -2);

    cout << "ceil" << endl;

    assert (IMATH_INTERNAL_NAMESPACE::ceil (0.0f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::ceil (0.5f) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::ceil (-0.5f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::ceil (1.0f) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::ceil (-1.0f) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::ceil (1.5f) == 2);
    assert (IMATH_INTERNAL_NAMESPACE::ceil (-1.5f) == -1);

    cout << "trunc" << endl;

    assert (IMATH_INTERNAL_NAMESPACE::trunc (0.0f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::trunc (0.5f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::trunc (-0.5f) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::trunc (1.0f) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::trunc (-1.0f) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::trunc (1.5f) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::trunc (-1.5f) == -1);

    cout << "divs / mods" << endl;

    assert (IMATH_INTERNAL_NAMESPACE::divs (5, 2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::mods (5, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (4, 2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::mods (4, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (3, 2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::mods (3, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (2, 2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::mods (2, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (1, 2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::mods (1, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (0, 2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::mods (0, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-1, 2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::mods (-1, 2) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-2, 2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::mods (-2, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-3, 2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::mods (-3, 2) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-4, 2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::mods (-4, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-5, 2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::mods (-5, 2) == -1);

    assert (IMATH_INTERNAL_NAMESPACE::divs (5, -2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::mods (5, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (4, -2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::mods (4, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (3, -2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::mods (3, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (2, -2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::mods (2, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (1, -2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::mods (1, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (0, -2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::mods (0, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-1, -2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::mods (-1, -2) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-2, -2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::mods (-2, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-3, -2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::mods (-3, -2) == -1);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-4, -2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::mods (-4, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divs (-5, -2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::mods (-5, -2) == -1);

    cout << "divp / modp" << endl;

    assert (IMATH_INTERNAL_NAMESPACE::divp (5, 2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::modp (5, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (4, 2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::modp (4, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (3, 2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::modp (3, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (2, 2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::modp (2, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (1, 2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::modp (1, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (0, 2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::modp (0, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-1, 2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::modp (-1, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-2, 2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::modp (-2, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-3, 2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::modp (-3, 2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-4, 2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::modp (-4, 2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-5, 2) == -3 &&
            IMATH_INTERNAL_NAMESPACE::modp (-5, 2) == 1);

    assert (IMATH_INTERNAL_NAMESPACE::divp (5, -2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::modp (5, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (4, -2) == -2 &&
            IMATH_INTERNAL_NAMESPACE::modp (4, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (3, -2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::modp (3, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (2, -2) == -1 &&
            IMATH_INTERNAL_NAMESPACE::modp (2, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (1, -2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::modp (1, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (0, -2) == 0 &&
            IMATH_INTERNAL_NAMESPACE::modp (0, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-1, -2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::modp (-1, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-2, -2) == 1 &&
            IMATH_INTERNAL_NAMESPACE::modp (-2, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-3, -2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::modp (-3, -2) == 1);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-4, -2) == 2 &&
            IMATH_INTERNAL_NAMESPACE::modp (-4, -2) == 0);
    assert (IMATH_INTERNAL_NAMESPACE::divp (-5, -2) == 3 &&
            IMATH_INTERNAL_NAMESPACE::modp (-5, -2) == 1);

    cout << "successor, predecessor" << endl;

    testf (0);
    testf (0.0 * -1.0);
    testf (1);
    testf (-1);
    testf (16);
    testf (7);
    testf (0.7);

    union {float f; int i;} u;
    u.i = 0x7f800000; //  inf
    testf (u.f, false);
    u.i = 0xff800000; // -inf
    testf (u.f, false);
    u.i = 0x7f800001; //  nan
    testf (u.f, false);
    u.i = 0x7f7fffff; //  FLT_MAX
    testf (u.f);
    u.i = 0xff7fffff; // -FLT_MAX
    testf (u.f);

    testd (0);
    testd (0.0 * -1.0);
    testd (1);
    testd (-1);
    testd (16);
    testd (7);
    testd (0.7);

    union {double d; Int64 i;} v;
    v.i = 0x7ff0000000000000ULL; //  inf
    testd (v.d, false);
    v.i = 0xfff0000000000000ULL; // -inf
    testd (v.d, false);
    v.i = 0x7ff0000000000001ULL; //  NAN
    testd (v.d, false);
    v.i = 0x7fefffffffffffffULL; //  FLT_MAX
    testd (v.d);
    v.i = 0xffefffffffffffffULL; // -FLT_MAX
    testd (v.d);

    cout << "ok\n" << endl;
}
