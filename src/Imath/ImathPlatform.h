//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

///
/// @file ImathPlatform.h
///
/// This file contains functions and constants which aren't
/// provided by the system libraries, compilers, or includes on
/// certain platforms.
///

#ifndef INCLUDED_IMATHPLATFORM_H
#define INCLUDED_IMATHPLATFORM_H

/// @cond Doxygen_Suppress

#include <math.h>
#include <type_traits>

#include "ImathNamespace.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

//
// Helpful macros for checking which C++ standard we are compiling with.
//
#if (__cplusplus >= 202002L)
#    define IMATH_CPLUSPLUS_VERSION 20
#elif (__cplusplus >= 201703L)
#    define IMATH_CPLUSPLUS_VERSION 17
#elif (__cplusplus >= 201402L) || (defined(_MSC_VER) && _MSC_VER >= 1914)
#    define IMATH_CPLUSPLUS_VERSION 14
#elif (__cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1900)
#    define IMATH_CPLUSPLUS_VERSION 11
#else
#    error "This version of Imath is meant to work only with C++11 and above"
#endif


//
// Constexpr C++14 conditional definition
//
#if (IMATH_CPLUSPLUS_VERSION >= 14)
  #define IMATH_CONSTEXPR14 constexpr
#else
  #define IMATH_CONSTEXPR14 /* can not be constexpr before c++14 */
#endif


//
// Define Imath::enable_if_t to be std for C++14, equivalent for C++11.
//
#if (IMATH_CPLUSPLUS_VERSION >= 14)
    using std::enable_if_t;    // Use C++14 std::enable_if_t
#else
    // Define enable_if_t for C++11
    template <bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;
#endif


//
// An enable_if helper to be used in template parameters which results in
// much shorter symbols.
//
#define IMATH_ENABLE_IF(...) Imath::enable_if_t<(__VA_ARGS__), int> = 0



#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#    define M_PI_2 1.57079632679489661923 // pi/2
#endif

//-----------------------------------------------------------------------------
//
//    Some, but not all, C++ compilers support the C99 restrict
//    keyword or some variant of it, for example, __restrict.
//
//-----------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER) || defined(__INTEL_COMPILER)
#    define IMATH_RESTRICT __restrict
#else
#    define IMATH_RESTRICT
#endif


IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

/// @endcond

#endif // INCLUDED_IMATHPLATFORM_H
