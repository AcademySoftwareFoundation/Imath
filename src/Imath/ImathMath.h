//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Obsolete functions provided for compatibility, deprecated in favor
// of std:: functions.
//

#ifndef INCLUDED_IMATHMATH_H
#define INCLUDED_IMATHMATH_H

#include "ImathNamespace.h"
#include "ImathPlatform.h"
#ifdef __CUDACC__
#   include <cuda/std/cmath>
#   include <cuda/std/limits>
#else
#   include <cmath>
#   include <limits>
#endif

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

//----------------------------------------------------------------------------
//
// The deprecated Math<T> methods were intended to allow templated access to
// math functions so that they would automatically choose either the double
// (e.g. sin()) or float (e.g., sinf()) version.
//
// Beginning wth C++11, this is unnecessary, as std:: versions of all these
// functions are available and are templated by type.
//
// We keep these old definitions for backward compatibility but encourage
// users to prefer the std:: versions. Some day we may remove these
// deprecated versions.
//
//----------------------------------------------------------------------------

/// @cond Doxygen_Suppress
template <class T> struct Math
{
    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T acos (T x) { return IMATH_STD_NAMESPACE::acos (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T asin (T x) { return IMATH_STD_NAMESPACE::asin (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T atan (T x) { return IMATH_STD_NAMESPACE::atan (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T atan2 (T x, T y) { return IMATH_STD_NAMESPACE::atan2 (x, y); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T cos (T x) { return IMATH_STD_NAMESPACE::cos (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T sin (T x) { return IMATH_STD_NAMESPACE::sin (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T tan (T x) { return IMATH_STD_NAMESPACE::tan (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T cosh (T x) { return IMATH_STD_NAMESPACE::cosh (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T sinh (T x) { return IMATH_STD_NAMESPACE::sinh (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T tanh (T x) { return IMATH_STD_NAMESPACE::tanh (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T exp (T x) { return IMATH_STD_NAMESPACE::exp (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T log (T x) { return IMATH_STD_NAMESPACE::log (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T log10 (T x) { return IMATH_STD_NAMESPACE::log10 (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T modf (T x, T* iptr)
    {
        T ival;
        T rval (IMATH_STD_NAMESPACE::modf (T (x), &ival));
        *iptr = ival;
        return rval;
    }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T pow (T x, T y) { return IMATH_STD_NAMESPACE::pow (x, y); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T sqrt (T x) { return IMATH_STD_NAMESPACE::sqrt (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T ceil (T x) { return IMATH_STD_NAMESPACE::ceil (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T fabs (T x) { return IMATH_STD_NAMESPACE::fabs (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T floor (T x) { return IMATH_STD_NAMESPACE::floor (x); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T fmod (T x, T y) { return IMATH_STD_NAMESPACE::fmod (x, y); }

    IMATH_DEPRECATED ("use std::math functions")
    IMATH_HOSTDEVICE
    static T hypot (T x, T y) { return IMATH_STD_NAMESPACE::hypot (x, y); }
};
/// @endcond

/// Don Hatch's version of sin(x)/x, which is accurate for very small x.
/// Returns 1 for x == 0.
template <class T>
IMATH_HOSTDEVICE inline T
sinx_over_x (T x)
{
    if (x * x < IMATH_STD_NAMESPACE::numeric_limits<T>::epsilon ())
        return T (1);
    else
        return IMATH_STD_NAMESPACE::sin (x) / x;
}

/// Compare two numbers and test if they are "approximately equal":
///
/// @return Ttrue if x1 is the same as x2 with an absolute error of
/// no more than e:
///
///	abs (x1 - x2) <= e
template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
equalWithAbsError (T x1, T x2, T e) IMATH_NOEXCEPT
{
    return ((x1 > x2) ? x1 - x2 : x2 - x1) <= e;
}

/// Compare two numbers and test if they are "approximately equal":
///
/// @return True if x1 is the same as x2 with an relative error of
/// no more than e,
///
/// abs (x1 - x2) <= e * x1
template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
equalWithRelError (T x1, T x2, T e) IMATH_NOEXCEPT
{
    return ((x1 > x2) ? x1 - x2 : x2 - x1) <= e * ((x1 > 0) ? x1 : -x1);
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHMATH_H
