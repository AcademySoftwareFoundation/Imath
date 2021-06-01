//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImath.h"
#include "PyImathExport.h"

namespace PyImath {

template <> PYIMATH_EXPORT const char * BoolArray::name()         { return "BoolArray"; }
template <> PYIMATH_EXPORT const char * SignedCharArray::name()   { return "SignedCharArray"; }
template <> PYIMATH_EXPORT const char * UnsignedCharArray::name() { return "UnsignedCharArray"; }
template <> PYIMATH_EXPORT const char * ShortArray::name()        { return "ShortArray"; }
template <> PYIMATH_EXPORT const char * UnsignedShortArray::name(){ return "UnsignedShortArray"; }
template <> PYIMATH_EXPORT const char * IntArray::name()          { return "IntArray"; }
template <> PYIMATH_EXPORT const char * UnsignedIntArray::name()  { return "UnsignedIntArray"; }
template <> PYIMATH_EXPORT const char * FloatArray::name()        { return "FloatArray"; }
template <> PYIMATH_EXPORT const char * DoubleArray::name()       { return "DoubleArray"; }
template <> PYIMATH_EXPORT const char * VIntArray::name()         { return "VIntArray"; }
template <> PYIMATH_EXPORT const char * VFloatArray::name()       { return "VFloatArray"; }
template <> PYIMATH_EXPORT const char * VV2iArray::name()         { return "VV2iArray"; }
template <> PYIMATH_EXPORT const char * VV2fArray::name()         { return "VV2fArray"; }

}
