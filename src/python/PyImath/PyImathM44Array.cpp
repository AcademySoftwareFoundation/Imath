//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyIlmBaseConfigInternal.h"

#include "PyImathM44Array.h"
#include "PyImathMatrix.h"
#include "PyImathExport.h"

namespace PyImath {
    
template<> PYIMATH_EXPORT const char*
M44dArray::name()
{
    return "M44dArray";
}

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Matrix44<double>
FixedArrayDefaultValue<IMATH_NAMESPACE::Matrix44<double> >::value()
{
    return IMATH_NAMESPACE::Matrix44<double>();
}
template<> PYIMATH_EXPORT  const char*
M44ArrayName<IMATH_NAMESPACE::M44d>::value()
{
    return "M44dArray";
}


} // namespace PyImath
