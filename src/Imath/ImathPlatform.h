///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IMATHPLATFORM_H
#define INCLUDED_IMATHPLATFORM_H

//----------------------------------------------------------------------------
//
//	ImathPlatform.h
//
//	This file contains functions and constants which aren't
//	provided by the system libraries, compilers, or includes on
//	certain platforms.
//
//----------------------------------------------------------------------------

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

#endif // INCLUDED_IMATHPLATFORM_H
