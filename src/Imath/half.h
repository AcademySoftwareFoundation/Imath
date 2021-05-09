//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Primary original authors:
//     Florian Kainz <kainz@ilm.com>
//     Rod Bogart <rgb@ilm.com>
//

#ifndef _HALF_H_
#define _HALF_H_

#include "ImathExport.h"
#include "ImathNamespace.h"
#include "ImathPlatform.h"

/// @file half.h
/// The conversion routines here have been extended to use the hardware
/// instructions when enabled by the compiler.
///
/// Further, Additional control is possible for embedded systems. First, the library can be configured to disable generation of the half to float table. This table is faster on x86 hardware, but for embedded systems may artificially bloat the executable. Without this, a smaller table is used for acceleration. An implementation can add a preprocessor #define IMATH_HALF_NO_TABLES_AT_ALL, which will eliminate all tables.
///
/// Testing on a Core i9, the timings are approximately:
///
/// half to float table: 0.69 ns / call
/// small table: 0.93 ns / call
/// no table: 1.36 ns / call
/// f16c: 0.45 ns / call
///
/// original: 5.2 ns / call
/// float to half: 4.57 ns / call**
/// no table: 1.48 ns / call**
/// f16c: 0.45 ns / call
///
/// ** - this depends on the distribution of the floats in question
///
/// Further, an implementation wishing to receive floating point
/// exceptions on underflow / overflow when converting float to half
/// can include this file with IMATH_HALF_EXCEPTIONS_ENABLED defined.
///
#if defined(__has_include)
#    if __has_include(<x86intrin.h>)
#        include <x86intrin.h>
#    elif __has_include(<intrin.h>)
#        include <intrin.h>
#    endif
#endif

//-------------------------------------------------------------------------
// Limits
//
// Visual C++ will complain if HALF_DENORM_MIN, HALF_NRM_MIN etc. are not float
// constants, but at least one other compiler (gcc 2.96) produces incorrect
// results if they are.
//-------------------------------------------------------------------------

#if (defined _WIN32 || defined _WIN64) && defined _MSC_VER

/// Smallest positive denormalized half
#    define HALF_DENORM_MIN 5.96046448e-08f

#    define HALF_NRM_MIN 6.10351562e-05f // Smallest positive normalized half

#    define HALF_MIN 6.10351562e-05f // Smallest positive normalized half

#    define HALF_MAX 65504.0f // Largest positive half

#    define HALF_EPSILON                                                                           \
        0.00097656f // Smallest positive e for which                                               \
                    // half (1.0 + e) != half (1.0)
#else

#    define HALF_DENORM_MIN 5.96046448e-08 // Smallest positive half

#    define HALF_NRM_MIN 6.10351562e-05 // Smallest positive normalized half

#    define HALF_MIN 6.10351562e-05f // Smallest positive normalized half

#    define HALF_MAX 65504.0 // Largest positive half

#    define HALF_EPSILON 0.00097656 // Smallest positive e for which
// half (1.0 + e) != half (1.0)
#endif

#define HALF_MANT_DIG                                                                              \
    11 // Number of digits in mantissa                                                            \
        // (significand + hidden leading 1)

//
// floor( (HALF_MANT_DIG - 1) * log10(2) ) => 3.01... -> 3
#define HALF_DIG                                                                                   \
    3 // Number of base 10 digits that                                                           \
        // can be represented without change

// ceil(HALF_MANT_DIG * log10(2) + 1) => 4.31... -> 5
#define HALF_DECIMAL_DIG                                                                           \
    5 // Number of base-10 digits that are                                                       \
        // necessary to uniquely represent all                                                     \
        // distinct values

#define HALF_RADIX 2 // Base of the exponent

#define HALF_DENORM_MIN_EXP                                                                        \
    -13 // Minimum negative integer such that                                                      \
        // HALF_RADIX raised to the power of                                                       \
        // one less than that integer is a                                                         \
        // normalized half

#define HALF_MAX_EXP                                                                               \
    16 // Maximum positive integer such that                                                      \
        // HALF_RADIX raised to the power of                                                       \
        // one less than that integer is a                                                         \
        // normalized half

#define HALF_DENORM_MIN_10_EXP                                                                     \
    -4 // Minimum positive integer such                                                           \
        // that 10 raised to that power is                                                         \
        // a normalized half

#define HALF_MAX_10_EXP                                                                            \
    4 // Maximum positive integer such                                                           \
        // that 10 raised to that power is                                                         \
        // a normalized half

#include <stdint.h>
#include <stdio.h>

#ifdef IMATH_HALF_EXCEPTIONS_ENABLED
#    include <fenv.h>
#endif

/** a type for both C-only programs and C++ to use the same utilities */
typedef union imath_half_uif
{
    uint32_t i;
    float f;
} imath_half_uif_t;

/** a type for both C-only programs and C++ to use the same utilities */
typedef uint16_t imath_half_bits_t;

#if !defined(__cplusplus) && !defined(__CUDACC__)
/** if we're in a C-only context, alias the half bits type to half */
typedef imath_half_bits_t half;
#endif

#if defined(IMATH_ENABLE_HALF_LOOKUP_TABLES)

#    if defined(__cplusplus)
extern "C"
#    else
extern
#    endif
    IMATH_EXPORT const imath_half_uif_t* imath_half_to_float_table;

// TODO: Can disappear once we remove original half conversion methods from the class
#        if defined(__cplusplus)
extern "C"
#        else
extern
#        endif
    IMATH_EXPORT const uint16_t* imath_float_half_exp_table;
#endif

////////////////////////////////////////

static inline float
imath_half_to_float (imath_half_bits_t h)
{
#if defined(__F16C__)
    // NB: The intel implementation does seem to treat NaN slightly
    // different than the original toFloat table does (i.e. where the
    // 1 bits are, meaning the signalling or not bits). This seems
    // benign, given that the original library didn't really deal with
    // signalling vs non-signalling NaNs
#    ifdef _MSC_VER
    /* msvc does not seem to have cvtsh_ss :( */
    return _mm_cvtss_f32 (_mm_cvtph_ps (_mm_set1_epi16 (h)));
#    else
    return _cvtsh_ss (h);
#    endif
#elif defined(IMATH_HALF_NO_TABLES_AT_ALL)
    imath_half_uif_t v;

    uint32_t hu32 = ((uint32_t) h) << 13;
    uint32_t sign = (hu32 & 0x10000000) << 3;
    uint32_t hexp = hu32 & 0x0f800000;
    uint32_t mant = hu32 & 0x007fe000;

    if (hexp == 0)
    {
        if (mant != 0)
        {
            // convert subnormal into normal float

            // 0x03ff << 13 is going to mean the first 9 bits are
            // meaningless in the count-the-leading-zero op below since
            // that's where the sign and exponent go, so those will be 0
            // but we want to get the mantissa / significand to the point
            // where it is no longer at 0.xxx but 1.xxx in radix 2, so we
            // need to add a digit of shifting so from 9 -> 8 to give us
            // one more shift
#    if defined(__GNUC__) || defined(__clang__)
            int lc = (int) __builtin_clz (mant);
            lc -= 8;
            hexp = (0x71 - lc) << 23;
            mant = (mant << lc);
#    else
            uint32_t xbit;
            hexp = 0x71 << 23;
            do
            {
                xbit = mant & 0x400000;
                mant <<= 1;
                hexp -= (1 << 23);
            } while (xbit == 0);
#    endif
            mant &= ~0x00800000;
        }
    }
    else if (hexp == 0x0f800000)
    {
        // inf or nan
        hexp = 0x7f800000; // 0xff << 23
        // set all the bits for NaN?
        //if ( mant != 0 )
        //    mant = 0x007fffff;
    }
    else
    {
        hexp += 0x38000000; // (0x70 << 23)
    }

    v.i = sign | hexp | mant;
    return v.f;
#elif defined(IMATH_ENABLE_HALF_LOOKUP_TABLES)
    return imath_half_to_float_table[h].f;
#else
    // clang-format off
#    define _IM_NEG(x) (((uint32_t)(1 << 31))|((uint32_t)x))
    static const uint32_t exps[64] = {
        0,
        ((127 - 14) << 23), ((127 - 13) << 23), ((127 - 12) << 23), ((127 - 11) << 23),
        ((127 - 10) << 23), ((127 - 9) << 23),  ((127 - 8) << 23), ((127 - 7) << 23),
        ((127 - 6) << 23), ((127 - 5) << 23), ((127 - 4) << 23), ((127 - 3) << 23),
        ((127 - 2) << 23), ((127 - 1) << 23), ((127 - 0) << 23), ((127 + 1) << 23),
        ((127 + 2) << 23), ((127 + 3) << 23), ((127 + 4) << 23), ((127 + 5) << 23),
        ((127 + 6) << 23), ((127 + 7) << 23), ((127 + 8) << 23), ((127 + 9) << 23),
        ((127 + 10) << 23), ((127 + 11) << 23), ((127 + 12) << 23), ((127 + 13) << 23),
        ((127 + 14) << 23), ((127 + 15) << 23),
        (0x000000ffU << 23),
        _IM_NEG(0),
        _IM_NEG((127 - 14) << 23), _IM_NEG((127 - 13) << 23), _IM_NEG((127 - 12) << 23), _IM_NEG((127 - 11) << 23),
        _IM_NEG((127 - 10) << 23), _IM_NEG((127 - 9) << 23), _IM_NEG((127 - 8) << 23), _IM_NEG((127 - 7) << 23),
        _IM_NEG((127 - 6) << 23), _IM_NEG((127 - 5) << 23), _IM_NEG((127 - 4) << 23), _IM_NEG((127 - 3) << 23),
        _IM_NEG((127 - 2) << 23), _IM_NEG((127 - 1) << 23), _IM_NEG((127 - 0) << 23), _IM_NEG((127 + 1) << 23),
        _IM_NEG((127 + 2) << 23), _IM_NEG((127 + 3) << 23), _IM_NEG((127 + 4) << 23), _IM_NEG((127 + 5) << 23),
        _IM_NEG((127 + 6) << 23), _IM_NEG((127 + 7) << 23), _IM_NEG((127 + 8) << 23), _IM_NEG((127 + 9) << 23),
        _IM_NEG((127 + 10) << 23), _IM_NEG((127 + 11) << 23), _IM_NEG((127 + 12) << 23), _IM_NEG((127 + 13) << 23),
        _IM_NEG((127 + 14) << 23), _IM_NEG((127 + 15) << 23),
        (0x000001ffU << 23)
    };
#    undef _IM_NEG
    // clang-format on
    imath_half_uif_t v;
    uint16_t he = (h >> 10);
    uint32_t m  = ((uint32_t) (h & 0x03ff)) << 13;

    // subnormal or zero
    if ((he & 0x1f) == 0)
    {
        // convert subnormal to normal
        if (m != 0)
        {
            uint32_t hexp;
            // 0x03ff << 13 is going to mean the first 9 bits are
            // meaningless in the count-the-leading-zero op below since
            // that's where the sign and exponent go, so those will be 0
            // but we want to get the mantissa / significand to the point
            // where it is no longer at 0.xxx but 1.xxx in radix 2, so we
            // need to add a digit of shifting so from 9 -> 8 to give us
            // one more shift
#    if defined(__GNUC__) || defined(__clang__)
            int lc = (int) __builtin_clz (m);
            lc -= 8;
            hexp = (0x71 - lc) << 23;
            m    = (m << lc);
#    else
            uint32_t xbit;
            hexp = 0x71 << 23;
            do
            {
                xbit = m & 0x400000;
                m <<= 1;
                hexp -= (1 << 23);
            } while (xbit == 0);
#    endif
            m &= ~0x00800000;
            v.i = (((uint32_t) (h & 0x8000)) << 16) | hexp | m;
        }
    }
    else
    {
        v.i = exps[he] | m;
    }
    return v.f;
#endif
}

/** NB: This only supports the one rounding mode which was present in
 * the original exr library (round to even) */
static inline imath_half_bits_t
imath_float_to_half (float f)
{
#if defined(__F16C__)
#    ifdef _MSC_VER
    /* msvc does not seem to have cvtsh_ss :( */
    return _mm_extract_epi16 (
        _mm_cvtps_ph (_mm_set_ss (f), (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)),
        0);
#    else
    /* preserve the fixed rounding mode to nearest */
    return _cvtss_sh (f, (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
#    endif
#else
    imath_half_uif_t v;
    imath_half_bits_t ret;
    uint32_t e, m, ui, r, shift;

    v.f = f;

    // TBD: This no longer seems faster...
//#    if !defined(IMATH_HALF_NO_TABLES_AT_ALL)
//#        if defined(IMATH_ENABLE_HALF_LOOKUP_TABLES)
//    e = imath_float_half_exp_table[(v.i >> 23)];
//    if (e != 0)
//    {
//        m = (v.i & 0x007fffff);
//        return e + ((m + 0x00000fff + ((m >> 13) & 1)) >> 13);
//    }
//#        endif
//#    endif
    ui  = (v.i & ~0x80000000);
    ret = ((v.i >> 16) & 0x8000);

    // zero or flush to 0
#    ifdef IMATH_HALF_EXCEPTIONS_ENABLED
    if (ui == 0)
        return ret;
    if (ui < 0x33000001)
    {
        feraiseexcept (FE_UNDERFLOW);
        return ret;
    }
#    else
    if (ui < 0x33000001)
        return ret;
#    endif

    if (ui >= 0x7f800000)
    {
        ret |= 0x7c00;
        if (ui == 0x7f800000)
            return ret;
        m = (ui & 0x7fffff) >> 13;
        // make sure we have at least one bit after shift to preserve nan-ness
        return ret | m | (m == 0);
    }

    // round to infinity
    if (ui > 0x477fefff)
    {
#    ifdef IMATH_HALF_EXCEPTIONS_ENABLED
        feraiseexcept (FE_OVERFLOW);
#    endif
        return ret | 0x7c00;
    }

    // exponent large enough to result in a normal number, round and return
    if (ui >= 0x38800000)
    {
        ui -= 0x38000000;
        ui = ((ui + 0x00000fff + ((ui >> 13) & 1)) >> 13);
        return ret | ui;
    }

    // produce a denormalized half
    e     = (ui >> 23);
    shift = 0x7e - e;
    m     = 0x800000 | (ui & 0x7fffff);
    r     = m << (32 - shift);
    ret |= (m >> shift);
    if (r > 0x80000000 || (r == 0x80000000 && (ret & 0x1) != 0))
        ++ret;
    return ret;
#endif
}

////////////////////////////////////////

#ifdef __cplusplus

#    include <iostream>

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

///
///
/// class half -- 16-bit floating point number
///
/// Type half can represent positive and negative numbers whose
/// magnitude is between roughly 6.1e-5 and 6.5e+4 with a relative
/// error of 9.8e-4; numbers smaller than 6.1e-5 can be represented
/// with an absolute error of 6.0e-8.  All integers from -2048 to
/// +2048 can be represented exactly.
///
/// Type half behaves (almost) like the built-in C++ floating point
/// types.  In arithmetic expressions, half, float and double can be
/// mixed freely.  Here are a few examples:
///
///     half a (3.5);
///     float b (a + sqrt (a));
///     a += b;
///     b += a;
///     b = a + 7;
///
/// Conversions from half to float are lossless; all half numbers
/// are exactly representable as floats.
///
/// Conversions from float to half may not preserve a float's value
/// exactly.  If a float is not representable as a half, then the
/// float value is rounded to the nearest representable half.  If a
/// float value is exactly in the middle between the two closest
/// representable half values, then the float value is rounded to
/// the closest half whose least significant bit is zero.
///
/// Overflows during float-to-half conversions cause arithmetic
/// exceptions.  An overflow occurs when the float value to be
/// converted is too large to be represented as a half, or if the
/// float value is an infinity or a NAN.
///
/// The implementation of type half makes the following assumptions
/// about the implementation of the built-in C++ types:
///
/// * float is an IEEE 754 single-precision number
/// * sizeof (float) == 4
/// * sizeof (unsigned int) == sizeof (float)
/// * alignof (unsigned int) == alignof (float)
/// * sizeof (uint16_t) == 2
///

class IMATH_EXPORT_TYPE half
{
  public:
    /// A special tag that lets us initialize a half from the raw bits.
    enum IMATH_EXPORT_ENUM FromBitsTag
    {
        FromBits
    };

    /// @{
    ///	@name Constructors

    /// Default construction provides no initialization (hence it is
    /// not constexpr).
    half() noexcept = default;

    /// Construct from float
    half (float f) noexcept;

    /// Construct from bit-vector
    constexpr half (FromBitsTag, uint16_t bits) noexcept;

    /// Copy constructor
    constexpr half (const half&) noexcept = default;

    /// Move constructor
    constexpr half (half&&) noexcept = default;

    /// Destructor
    ~half() noexcept = default;

    /// @}

    /// Conversion to float
    operator float() const noexcept;

    /// @{
    /// @name Basic Algebra

    /// Unary minus
    constexpr half operator-() const noexcept;

    /// Assignment
    half& operator= (const half& h) noexcept = default;

    /// Move assignment
    half& operator= (half&& h) noexcept = default;

    /// Assignment from float
    half& operator= (float f) noexcept;

    /// Addition assignment
    half& operator+= (half h) noexcept;

    /// Addition assignment from float
    half& operator+= (float f) noexcept;

    /// Subtraction assignment
    half& operator-= (half h) noexcept;

    /// Subtraction assignment from float
    half& operator-= (float f) noexcept;

    /// Multiplication assignment
    half& operator*= (half h) noexcept;

    /// Multiplication assignment from float
    half& operator*= (float f) noexcept;

    /// Division assignment
    half& operator/= (half h) noexcept;

    /// Division assignment from float
    half& operator/= (float f) noexcept;

    /// @}

    /// Round to n-bit precision (n should be between 0 and 10).
    /// After rounding, the significand's 10-n least significant
    /// bits will be zero.
    IMATH_CONSTEXPR14 half round (unsigned int n) const noexcept;

    /// @{
    /// @name Classification

    /// Return true if a normalized number, a denormalized number, or
    /// zero.
    constexpr bool isFinite() const noexcept;

    /// Return true if a normalized number.
    constexpr bool isNormalized() const noexcept;

    /// Return true if a denormalized number.
    constexpr bool isDenormalized() const noexcept;

    /// Return true if zero.
    constexpr bool isZero() const noexcept;

    /// Return true if NAN.
    constexpr bool isNan() const noexcept;

    /// Return true if a positive or a negative infinity
    constexpr bool isInfinity() const noexcept;

    /// Return true if the sign bit is set (negative)
    constexpr bool isNegative() const noexcept;

    /// @}

    /// @{
    /// @name Special values

    /// Return +infinity
    static constexpr half posInf() noexcept;

    /// Return -infinity
    static constexpr half negInf() noexcept;

    /// Returns a NAN with the bit pattern 0111111111111111
    static constexpr half qNan() noexcept;

    /// Return a NAN with the bit pattern 0111110111111111
    static constexpr half sNan() noexcept;

    /// @}

    /// @{
    /// @name Access to the internal representation

    /// Return the bit pattern
    IMATH_EXPORT constexpr uint16_t bits() const noexcept;

    /// Set the bit pattern
    IMATH_EXPORT IMATH_CONSTEXPR14 void setBits (uint16_t bits) noexcept;

    /// @}

  public:
    static_assert (sizeof (float) == sizeof (uint32_t),
                   "Assumption about the size of floats correct");
    using uif = imath_half_uif;

  private:
    //#ifdef IMATH_USE_ORIGINAL_HALF_IMPLEMENTATION
    IMATH_EXPORT static short convert (int i) noexcept;
    IMATH_EXPORT static float overflow() noexcept;
    //#endif
    constexpr uint16_t mantissa() const noexcept;
    constexpr uint16_t exponent() const noexcept;

    uint16_t _h;
};

//---------------------------------------------------------------------------
//
// Implementation --
//
// Representation of a float:
//
// We assume that a float, f, is an IEEE 754 single-precision
// floating point number, whose bits are arranged as follows:
//
//     31 (msb)
//     |
//     | 30     23
//     | |      |
//     | |      | 22                    0 (lsb)
//     | |      | |                     |
//     X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
//
//     s e        m
//
// S is the sign-bit, e is the exponent and m is the significand.
//
// If e is between 1 and 254, f is a normalized number:
//
//             s    e-127
//     f = (-1)  * 2      * 1.m
//
// If e is 0, and m is not zero, f is a denormalized number:
//
//             s    -126
//     f = (-1)  * 2      * 0.m
//
// If e and m are both zero, f is zero:
//
//     f = 0.0
//
// If e is 255, f is an "infinity" or "not a number" (NAN),
// depending on whether m is zero or not.
//
// Examples:
//
//     0 00000000 00000000000000000000000 = 0.0
//     0 01111110 00000000000000000000000 = 0.5
//     0 01111111 00000000000000000000000 = 1.0
//     0 10000000 00000000000000000000000 = 2.0
//     0 10000000 10000000000000000000000 = 3.0
//     1 10000101 11110000010000000000000 = -124.0625
//     0 11111111 00000000000000000000000 = +infinity
//     1 11111111 00000000000000000000000 = -infinity
//     0 11111111 10000000000000000000000 = NAN
//     1 11111111 11111111111111111111111 = NAN
//
// Representation of a half:
//
// Here is the bit-layout for a half number, h:
//
//     15 (msb)
//     |
//     | 14  10
//     | |   |
//     | |   | 9        0 (lsb)
//     | |   | |        |
//     X XXXXX XXXXXXXXXX
//
//     s e     m
//
// S is the sign-bit, e is the exponent and m is the significand.
//
// If e is between 1 and 30, h is a normalized number:
//
//             s    e-15
//     h = (-1)  * 2     * 1.m
//
// If e is 0, and m is not zero, h is a denormalized number:
//
//             S    -14
//     h = (-1)  * 2     * 0.m
//
// If e and m are both zero, h is zero:
//
//     h = 0.0
//
// If e is 31, h is an "infinity" or "not a number" (NAN),
// depending on whether m is zero or not.
//
// Examples:
//
//     0 00000 0000000000 = 0.0
//     0 01110 0000000000 = 0.5
//     0 01111 0000000000 = 1.0
//     0 10000 0000000000 = 2.0
//     0 10000 1000000000 = 3.0
//     1 10101 1111000001 = -124.0625
//     0 11111 0000000000 = +infinity
//     1 11111 0000000000 = -infinity
//     0 11111 1000000000 = NAN
//     1 11111 1111111111 = NAN
//
// Conversion:
//
// Converting from a float to a half requires some non-trivial bit
// manipulations.  In some cases, this makes conversion relatively
// slow, but the most common case is accelerated via table lookups.
//
// Converting back from a half to a float is easier because we don't
// have to do any rounding.  In addition, there are only 65536
// different half numbers; we can convert each of those numbers once
// and store the results in a table.  Later, all conversions can be
// done using only simple table lookups. (Although this is
// questionably the fastest way to do this, and it is implemented in
// hardware now)
//

//----------------------------
// Half-from-float constructor
//----------------------------

inline half::half (float f) noexcept
#    ifndef IMATH_USE_ORIGINAL_HALF_IMPLEMENTATION
    : _h (imath_float_to_half (f))
#    endif
{
#    ifdef IMATH_USE_ORIGINAL_HALF_IMPLEMENTATION
    uif x;

    x.f = f;

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

        e = imath_float_half_exp_table[e];

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

            _h = convert (x.i);
        }
    }
#    endif
}

//------------------------------------------
// Half from raw bits constructor
//------------------------------------------

inline constexpr half::half (FromBitsTag, uint16_t bits) noexcept : _h (bits)
{}

//------------------------------------------
// Half-to-float conversion via table lookup
//------------------------------------------

inline half::operator float() const noexcept
{
#    ifdef IMATH_USE_ORIGINAL_HALF_IMPLEMENTATION
    return imath_half_to_float_table[_h].f;
#    else
    return imath_half_to_float (_h);
#    endif
}

//-------------------------
// Round to n-bit precision
//-------------------------

inline IMATH_CONSTEXPR14 half
half::round (unsigned int n) const noexcept
{
    //
    // Parameter check.
    //

    if (n >= 10)
        return *this;

    //
    // Disassemble h into the sign, s,
    // and the combined exponent and significand, e.
    //

    uint16_t s = _h & 0x8000;
    uint16_t e = _h & 0x7fff;

    //
    // Round the exponent and significand to the nearest value
    // where ones occur only in the (10-n) most significant bits.
    // Note that the exponent adjusts automatically if rounding
    // up causes the significand to overflow.
    //

    e >>= 9 - n;
    e += e & 1;
    e <<= 9 - n;

    //
    // Check for exponent overflow.
    //

    if (e >= 0x7c00)
    {
        //
        // Overflow occurred -- truncate instead of rounding.
        //

        e = _h;
        e >>= 10 - n;
        e <<= 10 - n;
    }

    //
    // Put the original sign bit back.
    //

    half h (FromBits, s | e);

    return h;
}

//-----------------------
// Other inline functions
//-----------------------

inline constexpr half
half::operator-() const noexcept
{
    return half (FromBits, bits() ^ 0x8000);
}

inline half&
half::operator= (float f) noexcept
{
    *this = half (f);
    return *this;
}

inline half&
half::operator+= (half h) noexcept
{
    *this = half (float (*this) + float (h));
    return *this;
}

inline half&
half::operator+= (float f) noexcept
{
    *this = half (float (*this) + f);
    return *this;
}

inline half&
half::operator-= (half h) noexcept
{
    *this = half (float (*this) - float (h));
    return *this;
}

inline half&
half::operator-= (float f) noexcept
{
    *this = half (float (*this) - f);
    return *this;
}

inline half&
half::operator*= (half h) noexcept
{
    *this = half (float (*this) * float (h));
    return *this;
}

inline half&
half::operator*= (float f) noexcept
{
    *this = half (float (*this) * f);
    return *this;
}

inline half&
half::operator/= (half h) noexcept
{
    *this = half (float (*this) / float (h));
    return *this;
}

inline half&
half::operator/= (float f) noexcept
{
    *this = half (float (*this) / f);
    return *this;
}

inline constexpr uint16_t
half::mantissa() const noexcept
{
    return _h & 0x3ff;
}

inline constexpr uint16_t
half::exponent() const noexcept
{
    return (_h >> 10) & 0x001f;
}

inline constexpr bool
half::isFinite() const noexcept
{
    return exponent() < 31;
}

inline constexpr bool
half::isNormalized() const noexcept
{
    return exponent() > 0 && exponent() < 31;
}

inline constexpr bool
half::isDenormalized() const noexcept
{
    return exponent() == 0 && mantissa() != 0;
}

inline constexpr bool
half::isZero() const noexcept
{
    return (_h & 0x7fff) == 0;
}

inline constexpr bool
half::isNan() const noexcept
{
    return exponent() == 31 && mantissa() != 0;
}

inline constexpr bool
half::isInfinity() const noexcept
{
    return exponent() == 31 && mantissa() == 0;
}

inline constexpr bool
half::isNegative() const noexcept
{
    return (_h & 0x8000) != 0;
}

inline constexpr half
half::posInf() noexcept
{
    return half (FromBits, 0x7c00);
}

inline constexpr half
half::negInf() noexcept
{
    return half (FromBits, 0xfc00);
}

inline constexpr half
half::qNan() noexcept
{
    return half (FromBits, 0x7fff);
}

inline constexpr half
half::sNan() noexcept
{
    return half (FromBits, 0x7dff);
}

inline constexpr uint16_t
half::bits() const noexcept
{
    return _h;
}

inline IMATH_CONSTEXPR14 void
half::setBits (uint16_t bits) noexcept
{
    _h = bits;
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

//-----------
// Stream I/O
//-----------

IMATH_EXPORT std::ostream& operator<< (std::ostream& os, IMATH_INTERNAL_NAMESPACE::half h);
IMATH_EXPORT std::istream& operator>> (std::istream& is, IMATH_INTERNAL_NAMESPACE::half& h);

//----------
// Debugging
//----------

IMATH_EXPORT void printBits (std::ostream& os, IMATH_INTERNAL_NAMESPACE::half h);
IMATH_EXPORT void printBits (std::ostream& os, float f);
IMATH_EXPORT void printBits (char c[19], IMATH_INTERNAL_NAMESPACE::half h);
IMATH_EXPORT void printBits (char c[35], float f);

#    ifndef __CUDACC__
using half = IMATH_INTERNAL_NAMESPACE::half;
#    else
#        include <cuda_fp16.h>
#    endif

#endif // __cplusplus

#endif
