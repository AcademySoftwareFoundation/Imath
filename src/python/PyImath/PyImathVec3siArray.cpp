//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec3ArrayImpl.h"
#include "PyImathExport.h"

namespace PyImath {
using namespace boost::python;
using namespace IMATH_NAMESPACE;

template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec3<unsigned char> > > register_Vec3Array<unsigned char>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec3<short> > > register_Vec3Array<short>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec3<int> > > register_Vec3Array<int>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec3<int64_t> > > register_Vec3Array<int64_t>();

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec3<unsigned char> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec3<unsigned char> >::value() { return IMATH_NAMESPACE::Vec3<unsigned char>(0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec3<short>         PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec3<short> >::value() { return IMATH_NAMESPACE::Vec3<short>(0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec3<int>           PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec3<int> >::value() { return IMATH_NAMESPACE::Vec3<int>(0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec3<int64_t>       PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Vec3<int64_t> >::value() { return IMATH_NAMESPACE::Vec3<int64_t>(0,0,0); }
}
