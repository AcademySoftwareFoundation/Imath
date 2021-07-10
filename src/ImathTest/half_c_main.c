//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// For the C version test, omit the lookup table, which validates that
// ``half.h`` works as a "header-only" implementation not requiring
// the compiled library. Note that the C-language support for half
// only includes conversion to and from float.
#define IMATH_HALF_NO_LOOKUP_TABLE

#include <half.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

typedef union
{
    uint32_t i;
    float f;
} c_half_uif;

static const c_half_uif half_to_float[1 << 16] =
#include "../Imath/toFloat.h"

static const unsigned short half_eLut[1 << 9] =
#include "eLut.h"

static short exp_long_convert (int i)
{
    int s = (i >> 16) & 0x00008000;
    int e = ((i >> 23) & 0x000000ff) - (127 - 15);
    int m = i & 0x007fffff;

    //fprintf( stderr, "old_convert: s %d   e = %d, m = %d\n", s, e, m );
    if (e <= 0)
    {
        if (e < -10)
        {
            return s;
        }

        m = m | 0x00800000;

        int t = 14 - e;
        int a = (1 << (t - 1)) - 1;
        int b = (m >> t) & 1;

        m = (m + a + b) >> t;

        //fprintf( stderr, " <OLD> e %d, m: 0x%08X, t %d, a: %d 0x%08X, b: %d 0x%08X\n", e, m, t, a, a, b, b );
        return s | m;
    }
    else if (e == 0xff - (127 - 15))
    {
        if (m == 0)
        {
            return s | 0x7c00;
        }
        else
        {
            m >>= 13;
            return s | 0x7c00 | m | (m == 0);
        }
    }
    else
    {
        m = m + 0x00000fff + ((m >> 13) & 1);

        if (m & 0x00800000)
        {
            m = 0;  // overflow in significand,
            e += 1; // adjust exponent
        }
        if (e > 30)
        {
            return s | 0x7c00; // if this returns, the half becomes an
        }                      // infinity with the same sign as f.
        return s | (e << 10) | (m >> 13);
    }
}

static uint16_t
exptable_method (float f)
{
    c_half_uif x;
    uint16_t _h = 0;
    x.f         = f;

    if (f == 0)
    {
        //
        // Common special case - zero.
        // Preserve the zero's sign bit.
        //

        _h = (x.i >> 16);
    }
    else
    {
        //
        // We extract the combined sign and exponent, e, from our
        // floating-point number, f.  Then we convert e to the sign
        // and exponent of the half number via a table lookup.
        //
        // For the most common case, where a normalized half is produced,
        // the table lookup returns a non-zero value; in this case, all
        // we have to do is round f's significand to 10 bits and combine
        // the result with e.
        //
        // For all other cases (overflow, zeroes, denormalized numbers
        // resulting from underflow, infinities and NANs), the table
        // lookup returns zero, and we call a longer, non-inline function
        // to do the float-to-half conversion.
        //

        int e = (x.i >> 23) & 0x000001ff;

        e = half_eLut[e];

        if (e)
        {
            //
            // Simple case - round the significand, m, to 10
            // bits and combine it with the sign and exponent.
            //

            int m = x.i & 0x007fffff;
            _h    = e + ((m + 0x00000fff + ((m >> 13) & 1)) >> 13);
        }
        else
        {
            //
            // Difficult case - call a function.
            //
            _h = exp_long_convert (x.i);
        }
    }
    return _h;
}

int
main (int argc, char* argv[])
{
    int ret = 0;
    c_half_uif conv;
    half test, test2;
    conv.f = HALF_DENORM_MIN + HALF_DENORM_MIN * 0.5f;
    test   = imath_float_to_half (conv.f);
    test2  = exptable_method (conv.f);
    if (test != test2)
    {
        fprintf (stderr,
                 "Invalid conversion of %.10g 0x%08X 0x%08X downconvert 0x%04X vs 0x%04X\n",
                 conv.f,
                 (conv.i >> 13) & 0x3ff,
                 (conv.i >> 13) & 0x3ff,
                 test,
                 test2);
        ret = 1;
    }

    int diffcount = 0;
    for (int i = 0; i < (1 << 16); ++i)
    {
        conv.f = imath_half_to_float ((half) i);
        if (conv.i != half_to_float[i].i)
        {
            uint16_t h  = (uint16_t) i;
            uint16_t he = (h >> 10) & 0x1f;
            uint16_t hm = (h & 0x3ff);

#ifdef __F16C__
            // the intel instructions do something different w/ NaN values than the original half library
            if (he == 0x1f && isnan (conv.f))
            {
                ++diffcount;
                continue;
            }
#endif
            fprintf (
                stderr,
                "half to float %d: C gives %.10f (0x%08X) vs %.10f (0x%08X) [h 0x%04X he 0x%04X hm 0x%04X]\n",
                i,
                conv.f,
                conv.i,
                half_to_float[i].f,
                half_to_float[i].i,
                h,
                he,
                hm);
            ret = 1;
        }
    }
    if (diffcount != 0)
        fprintf (
            stderr,
            "WARNING: Seems safe, but %d NaN values were different between hardware implementation and library implementation\n",
            diffcount);

    for (int i = 0; i < (1 << 16); ++i)
    {
        conv = half_to_float[i];
        test = imath_float_to_half (conv.f);
        if (test != i)
        {
            // well, we ensure that nan stays nan after conversion by
            // adding a low bit, so it won't always align
            int e = (conv.i >> 23) & 0xff;
            int m = (conv.i & 0x007fffff);
            if (e == 255 && m != 0)
                continue;

            fprintf (stderr,
                     "float to half %d: %.10f (0x%08X) gives %d 0x%04X (e is %d)\n",
                     i,
                     conv.f,
                     conv.i,
                     (int) test,
                     test,
                     e);
            ret = 1;
        }
    }

    return ret;
}
