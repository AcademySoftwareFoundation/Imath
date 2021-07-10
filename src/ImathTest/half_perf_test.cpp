//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef _MSC_VER
#    define _CRT_RAND_S
#endif

#include <ImathConfig.h>
#include <half.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#ifdef _MSC_VER
#    include <windows.h>
#else
#    include <time.h>
#endif

#include <memory>

static const unsigned short imath_float_half_exp_table[1 << 9] =
#include "eLut.h"

using namespace IMATH_NAMESPACE;

#ifdef IMATH_USE_HALF_LOOKUP_TABLE

static inline float table_half_cast(const half &h)
{
    return imath_half_to_float_table[h.bits()].f;
}

//-----------------------------------------------
// Overflow handler for float-to-half conversion;
// generates a hardware floating-point overflow,
// which may be trapped by the operating system.
//-----------------------------------------------

static float
half_overflow()
{
    float f = 1e10;

    for (int i = 0; i < 10; i++)
        f *= f; // this will overflow before the for loop terminates

    return f;
}

//-----------------------------------------------------
// Float-to-half conversion -- general case, including
// zeroes, denormalized numbers and exponent overflows.
//-----------------------------------------------------

static uint16_t
long_convert (int i)
{
    //
    // Our floating point number, f, is represented by the bit
    // pattern in integer i.  Disassemble that bit pattern into
    // the sign, s, the exponent, e, and the significand, m.
    // Shift s into the position where it will go in in the
    // resulting half number.
    // Adjust e, accounting for the different exponent bias
    // of float and half (127 versus 15).
    //

    int s =  (i >> 16) & 0x00008000;
    int e = ((i >> 23) & 0x000000ff) - (127 - 15);
    int m =   i        & 0x007fffff;

    //
    // Now reassemble s, e and m into a half:
    //

    if (e <= 0)
    {
        if (e < -10)
        {
            //
            // E is less than -10.  The absolute value of f is
            // less than HALF_DENORM_MIN (f may be a small normalized
            // float, a denormalized float or a zero).
            //
            // We convert f to a half zero with the same sign as f.
            //

            return s;
        }

        //
        // E is between -10 and 0.  F is a normalized float
        // whose magnitude is less than HALF_NRM_MIN.
        //
        // We convert f to a denormalized half.
        //

        //
        // Add an explicit leading 1 to the significand.
        //

        m = m | 0x00800000;

        //
        // Round to m to the nearest (10+e)-bit value (with e between
        // -10 and 0); in case of a tie, round to the nearest even value.
        //
        // Rounding may cause the significand to overflow and make
        // our number normalized.  Because of the way a half's bits
        // are laid out, we don't have to treat this case separately;
        // the code below will handle it correctly.
        //

        int t = 14 - e;
        int a = (1 << (t - 1)) - 1;
        int b = (m >> t) & 1;

        m = (m + a + b) >> t;

        //
        // Assemble the half from s, e (zero) and m.
        //

        return s | m;
    }
    else if (e == 0xff - (127 - 15))
    {
        if (m == 0)
        {
            //
            // F is an infinity; convert f to a half
            // infinity with the same sign as f.
            //

            return s | 0x7c00;
        }
        else
        {
            //
            // F is a NAN; we produce a half NAN that preserves
            // the sign bit and the 10 leftmost bits of the
            // significand of f, with one exception: If the 10
            // leftmost bits are all zero, the NAN would turn
            // into an infinity, so we have to set at least one
            // bit in the significand.
            //

            m >>= 13;
            return s | 0x7c00 | m | (m == 0);
        }
    }
    else
    {
        //
        // E is greater than zero.  F is a normalized float.
        // We try to convert f to a normalized half.
        //

        //
        // Round to m to the nearest 10-bit value.  In case of
        // a tie, round to the nearest even value.
        //

        m = m + 0x00000fff + ((m >> 13) & 1);

        if (m & 0x00800000)
        {
            m = 0;  // overflow in significand,
            e += 1; // adjust exponent
        }

        //
        // Handle exponent overflow
        //

        if (e > 30)
        {
            half_overflow ();        // Cause a hardware floating point overflow;
            return s | 0x7c00; // if this returns, the half becomes an
        }                      // infinity with the same sign as f.

        //
        // Assemble the half from s, e and m.
        //

        return s | (e << 10) | (m >> 13);
    }
}

static inline half exptable_half_constructor(float f)
{
    half ret;
    imath_half_uif x;

    x.f = f;

    if (f == 0)
    {
        //
        // Common special case - zero.
        // Preserve the zero's sign bit.
        //

        ret.setBits( (x.i >> 16) );
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

        e = imath_float_half_exp_table[e];

        if (e)
        {
            //
            // Simple case - round the significand, m, to 10
            // bits and combine it with the sign and exponent.
            //

            int m = x.i & 0x007fffff;
            ret.setBits (e + ((m + 0x00000fff + ((m >> 13) & 1)) >> 13));
        }
        else
        {
            //
            // Difficult case - call a function.
            //

            ret.setBits (long_convert (x.i));
        }
    }
    return ret;
}
#else
// provide a wrapping function for consistency/readability
static inline float table_half_cast(const half &h)
{
    return static_cast<float>( h );
}

static inline half exptable_half_constructor(float f)
{
    return half {f};
}

#endif

int64_t
get_ticks (void)
{
#ifdef _MSC_VER
    static uint64_t scale = 0;
    if (scale == 0)
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency (&freq);
        scale = (1000000000 / freq.QuadPart);
    }

    LARGE_INTEGER ticks;
    QueryPerformanceCounter (&ticks);
    return ticks.QuadPart * scale;
#else
    struct timespec t;
    uint64_t nsecs;

    static uint64_t start = 0;
    if (start == 0)
    {
        clock_gettime (CLOCK_MONOTONIC, &t);
        start = t.tv_sec;
    }

    clock_gettime (CLOCK_MONOTONIC, &t);
    nsecs = (t.tv_sec - start) * 1000000000;
    nsecs += t.tv_nsec;
    return nsecs;
#endif
}

void
perf_test_half_to_float (float* floats, const uint16_t* halfs, int numentries)
{
    const half* halfvals = reinterpret_cast<const half*> (halfs);

    int64_t st = get_ticks();
    for (int i = 0; i < numentries; ++i)
        floats[i] = imath_half_to_float (halfs[i]);
    int64_t et = get_ticks();

    int64_t ost = get_ticks();
    for (int i = 0; i < numentries; ++i)
        floats[i] = table_half_cast (halfvals[i]);
    int64_t oet = get_ticks();

    int64_t onanos = (oet - ost);
    int64_t nnanos = (et - st);
    fprintf (stderr,
             "half -> float Old: %10lld (%g ns) New: %10lld (%g ns) (%10lld)\n",
             (long long) onanos,
             (double) onanos / ((double) numentries),
             (long long) nnanos,
             (double) nnanos / ((double) numentries),
             ((long long) (onanos - nnanos)));
}

void
perf_test_float_to_half (uint16_t* halfs, const float* floats, int numentries)
{
    half* halfvals = reinterpret_cast<half*> (halfs);

    int64_t st = get_ticks();
    for (int i = 0; i < numentries; ++i)
        halfs[i] = imath_float_to_half (floats[i]);
    int64_t et = get_ticks();

    int64_t ost = get_ticks();
    for (int i = 0; i < numentries; ++i)
        halfvals[i] = exptable_half_constructor (floats[i]);
    int64_t oet = get_ticks();

    int64_t onanos = (oet - ost);
    int64_t nnanos = (et - st);
    fprintf (stderr,
             "float -> half Old: %10lld (%g ns) New: %10lld (%g ns) (%10lld)\n",
             (long long) onanos,
             (double) onanos / ((double) numentries),
             (long long) nnanos,
             (double) nnanos / ((double) numentries),
             ((long long) (onanos - nnanos)));
}

int
main (int argc, char* argv[])
{
    int ret        = 0;
    int numentries = 1920 * 1080 * 3;
    if (argc > 1)
    {
        numentries = atoi (argv[1]);

        if (numentries <= 0)
        {
            fprintf (stderr, "Bad entry count '%s'\n", argv[1]);
            ret = 1;
        }
    }

    if (numentries > 0)
    {
        uint16_t* halfs = new uint16_t[numentries];
        float* floats   = new float[numentries];

        if (halfs && floats)
        {
            srand (numentries);
            for (int i = 0; i < numentries; ++i)
            {
                halfs[i]  = (uint16_t) (rand());
                floats[i] = imath_half_to_float (halfs[i]);
            }
            perf_test_half_to_float (floats, halfs, numentries);

            // test float -> half with real-world values
#ifdef _MSC_VER
            unsigned int rv;
            for (int i = 0; i < numentries; ++i)
            {
                rand_s( &rv );
                floats[i] = 65504.0 * (((double) rand() / (double) UINT_MAX) * 2.0 - 1.0);
            }
#else
            for (int i = 0; i < numentries; ++i)
                floats[i] = 65504.0 * (drand48() * 2.0 - 1.0);
#endif
            perf_test_float_to_half (halfs, floats, numentries);
        }

        delete[] halfs;
        delete[] floats;
    }

    return ret;
}
