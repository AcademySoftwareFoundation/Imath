//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Primary original authors:
//     Florian Kainz <kainz@ilm.com>
//     Rod Bogart <rgb@ilm.com>
//


//---------------------------------------------------------------------------
//
//	class half --
//	implementation of non-inline members
//
//---------------------------------------------------------------------------

#include "half.h"
#include <assert.h>

using namespace std;

#if defined(IMATH_DLL)
#    define EXPORT_CONST __declspec(dllexport)
#else
#    define EXPORT_CONST
#endif

//-------------------------------------------------------------
// Lookup tables for half-to-float and float-to-half conversion
//-------------------------------------------------------------

// clang-format off

EXPORT_CONST const half::uif half::_toFloat[1 << 16] =
#include "toFloat.h"
EXPORT_CONST const unsigned short half::_eLut[1 << 9] =
#include "eLut.h"

//-----------------------------------------------
// Overflow handler for float-to-half conversion;
// generates a hardware floating-point overflow,
// which may be trapped by the operating system.
//-----------------------------------------------

IMATH_EXPORT float
half::overflow() noexcept
{
    volatile float f = 1e10;

    for (int i = 0; i < 10; i++)
        f *= f; // this will overflow before the for loop terminates

    return f;
}

//-----------------------------------------------------
// Float-to-half conversion -- general case, including
// zeroes, denormalized numbers and exponent overflows.
//-----------------------------------------------------

IMATH_EXPORT short
half::convert (int i) noexcept
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
            overflow();        // Cause a hardware floating point overflow;
            return s | 0x7c00; // if this returns, the half becomes an
        }                      // infinity with the same sign as f.

        //
        // Assemble the half from s, e and m.
        //

        return s | (e << 10) | (m >> 13);
    }
}

//---------------------
// Stream I/O operators
//---------------------

IMATH_EXPORT ostream&
operator<< (ostream& os, half h)
{
    os << float (h);
    return os;
}

IMATH_EXPORT istream&
operator>> (istream& is, half& h)
{
    float f;
    is >> f;
    h = half (f);
    return is;
}

//---------------------------------------
// Functions to print the bit-layout of
// floats and halfs, mostly for debugging
//---------------------------------------

IMATH_EXPORT void
printBits (ostream& os, half h)
{
    unsigned short b = h.bits();

    for (int i = 15; i >= 0; i--)
    {
        os << (((b >> i) & 1) ? '1' : '0');

        if (i == 15 || i == 10)
            os << ' ';
    }
}

IMATH_EXPORT void
printBits (ostream& os, float f)
{
    half::uif x;
    x.f = f;

    for (int i = 31; i >= 0; i--)
    {
        os << (((x.i >> i) & 1) ? '1' : '0');

        if (i == 31 || i == 23)
            os << ' ';
    }
}

IMATH_EXPORT void
printBits (char c[19], half h)
{
    unsigned short b = h.bits();

    for (int i = 15, j = 0; i >= 0; i--, j++)
    {
        c[j] = (((b >> i) & 1) ? '1' : '0');

        if (i == 15 || i == 10)
            c[++j] = ' ';
    }

    c[18] = 0;
}

IMATH_EXPORT void
printBits (char c[35], float f)
{
    half::uif x;
    x.f = f;

    for (int i = 31, j = 0; i >= 0; i--, j++)
    {
        c[j] = (((x.i >> i) & 1) ? '1' : '0');

        if (i == 31 || i == 23)
            c[++j] = ' ';
    }

    c[34] = 0;
}
