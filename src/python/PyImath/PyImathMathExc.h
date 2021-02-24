//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathMathExc_h_
#define _PyImathMathExc_h_

//
// Note: when PyImath from the v2 release of OpenEXR depended on Iex,
// the MATH_EXC_ON macro enabled float-point exceptions via the
// MathExcOn class. This was a compile-time option based on the
// setting of PYIMATH_ENABLE_EXCEPTIONS. This behavior is now
// deprecated, hence the empty macro.
//

#define MATH_EXC_ON

#endif
