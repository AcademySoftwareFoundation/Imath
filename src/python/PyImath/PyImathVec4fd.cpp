//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec4Impl.h"
#include "PyImathVec4ArrayImpl.h"
#include "PyImathExport.h"

namespace PyImath {
template <> const char *PyImath::V4fArray::name() { return "V4fArray"; }
template <> const char *PyImath::V4dArray::name() { return "V4dArray"; }

using namespace boost::python;
using namespace IMATH_NAMESPACE;

template<> const char *Vec4Name<float>::value() { return "V4f"; }
template<> const char *Vec4Name<double>::value() { return "V4d"; }

// Specialization for float to full precision
template <>
std::string Vec4_repr(const Vec4<float> &v)
{
    return (boost::format("%s(%.9g, %.9g, %.9g, %.9g)")
                        % Vec4Name<float>::value() % v.x % v.y % v.z % v.w).str();
}

// Specialization for double to full precision
template <>
std::string Vec4_repr(const Vec4<double> &v)
{
    return (boost::format("%s(%.17g, %.17g, %.17g, %.17g)")
                        % Vec4Name<double>::value() % v.x % v.y % v.z % v.w).str();
}

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec4<float> > register_Vec4<float>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec4<double> > register_Vec4<double>();
		 
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec4<float> > > register_Vec4Array<float>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec4<double> > > register_Vec4Array<double>();

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec4<float> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec4<float> >::value() { return IMATH_NAMESPACE::Vec4<float>(0,0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec4<double> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec4<double> >::value() { return IMATH_NAMESPACE::Vec4<double>(0,0,0,0); }
}
