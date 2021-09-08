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

    union {float f; uint32_t i;} u;
    u.f = f;
    printf ("f %.9g %x\n", f, u.i);
    u.f = sf;
    printf ("sf %.9g %x\n", sf, u.i);
    u.f = pf;
    printf ("pf %.9g %x\n", pf, u.i);
    u.f = spf;
    printf ("spf %.9g %x\n", spf, u.i);
    u.f = psf;
    printf ("psf %.9g %x\n", psf, u.i);

    fflush (stdout);

    if (changeExpected)
    {
        assert (pf < f);
        assert (f < sf);
    }
    else
    {
        // No bit change expected if input was inf or NaN
        uint32_t bc_f = bit_cast<uint32_t> (f);
        uint32_t bc_pf = bit_cast<uint32_t> (pf);
        uint32_t bc_sf = bit_cast<uint32_t> (sf);
        
        if (isnan(f))
        {
            printf ("no change expected [isnan(f)]: f=%x pf=%x sf=%x\n", bc_f, bc_pf, bc_sf);
            assert (isnan(pf));
            assert (isnan(sf));
        }
        else
        {
            printf ("no change expected: [!isnan(f)]: f=%x pf=%x sf=%x\n", bc_f, bc_pf, bc_sf);
            assert (bit_cast<uint32_t> (pf) == bit_cast<uint32_t> (f));
            assert (bit_cast<uint32_t> (sf) == bit_cast<uint32_t> (f));
        }
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

    union {double d; uint64_t i;} u;

    u.d = d;
    printf ("d %.18lg %lx\n", d, u.i);
    u.d = sd;
    printf ("sd %.18lg %lx\n", sd, u.i);
    u.d = pd;
    printf ("pd %.18lg %lx\n", pd, u.i);
    u.d = spd;
    printf ("spd %.18lg %lx\n", spd, u.i);
    u.d = psd;
    printf ("psd %.18lg %lx\n", psd, u.i);

    fflush (stdout);

    if (changeExpected)
    {
        assert (pd < d);
        assert (d < sd);
    }
    else
    {
        uint64_t bc_d = bit_cast<uint64_t> (d);
        uint64_t bc_pd = bit_cast<uint64_t> (pd);
        uint64_t bc_sd = bit_cast<uint64_t> (sd);
        printf ("no change expected: d=%lx pd=%lx sd=%lx\n", bc_d, bc_pd, bc_sd);

        // No bit change expected if input was inf or NaN
        assert (bit_cast<uint64_t> (pd) == bit_cast<uint64_t> (d));
        assert (bit_cast<uint64_t> (sd) == bit_cast<uint64_t> (d));

        if (isnan(d))
        {
            printf ("no change expected [isnan(d)]: d=%lx pd=%lx sd=%lx\n", bc_d, bc_pd, bc_sd);
            assert (isnan(pd));
            assert (isnan(sd));
        }
        else
        {
            printf ("no change expected: [!isnan(d)]: d=%lx pd=%lx sd=%lx\n", bc_d, bc_pd, bc_sd);
            assert (bit_cast<uint64_t> (pd) == bit_cast<uint64_t> (d));
            assert (bit_cast<uint64_t> (sd) == bit_cast<uint64_t> (d));
        }
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

    union {float f; uint32_t i;} u;
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

    union {double d; uint64_t i;} v;
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
