//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec2Impl.h"
#include "PyImathExport.h"

namespace PyImath {
template <> const char *PyImath::V2fArray::name() { return "V2fArray"; }
template <> const char *PyImath::V2dArray::name() { return "V2dArray"; }

using namespace boost::python;
using namespace IMATH_NAMESPACE;

template<> const char *Vec2Name<float>::value  = "V2f";
template<> const char *Vec2Name<double>::value = "V2d";

// Specialization for float to full precision
template <>
std::string Vec2_repr(const Vec2<float> &v)
{
    return (boost::format("%s(%.9g, %.9g)")
                        % Vec2Name<float>::value % v.x % v.y).str();
}

// Specialization for double to full precision
template <>
std::string Vec2_repr(const Vec2<double> &v)
{
    return (boost::format("%s(%.17g, %.17g)")
                        % Vec2Name<double>::value % v.x % v.y).str();
}

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec2<float> > register_Vec2<float>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec2<double> > register_Vec2<double>();
		 
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec2<float> > > register_Vec2Array<float>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec2<double> > > register_Vec2Array<double>();

template<> IMATH_NAMESPACE::Vec2<float> PYIMATH_EXPORT FixedArrayDefaultValue<IMATH_NAMESPACE::Vec2<float> >::value() { return IMATH_NAMESPACE::Vec2<float>(0,0); }
template<> IMATH_NAMESPACE::Vec2<double> PYIMATH_EXPORT FixedArrayDefaultValue<IMATH_NAMESPACE::Vec2<double> >::value() { return IMATH_NAMESPACE::Vec2<double>(0,0); }
}

