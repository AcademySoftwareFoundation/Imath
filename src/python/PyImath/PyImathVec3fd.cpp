//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec3Impl.h"
#include "PyImathVec3ArrayImpl.h"
#include "PyImathExport.h"

namespace PyImath {
template <> const char *PyImath::V3fArray::name() { return "V3fArray"; }
template <> const char *PyImath::V3dArray::name() { return "V3dArray"; }

using namespace boost::python;
using namespace IMATH_NAMESPACE;

template<> const char *Vec3Name<float>::value() { return "V3f"; }
template<> const char *Vec3Name<double>::value() { return "V3d"; }

// Specialization for float to full precision
template <>
std::string Vec3_repr(const Vec3<float> &v)
{
    return (boost::format("%s(%.9g, %.9g, %.9g)")
                        % Vec3Name<float>::value() % v.x % v.y % v.z).str();
}

// Specialization for double to full precision
template <>
std::string Vec3_repr(const Vec3<double> &v)
{
    return (boost::format("%s(%.17g, %.17g, %.17g)")
                        % Vec3Name<double>::value() % v.x % v.y % v.z).str();
}

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec3<float> > register_Vec3<float>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec3<double> > register_Vec3<double>();
		 
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec3<float> > > register_Vec3Array<float>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec3<double> > > register_Vec3Array<double>();

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec3<float> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec3<float> >::value() { return IMATH_NAMESPACE::Vec3<float>(0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec3<double> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec3<double> >::value() { return IMATH_NAMESPACE::Vec3<double>(0,0,0); }
}
