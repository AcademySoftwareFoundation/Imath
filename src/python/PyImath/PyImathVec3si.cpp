//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec3Impl.h"
#include "PyImathExport.h"

namespace PyImath {
template <> const char *PyImath::V3cArray::name()   { return "V3cArray"; }
template <> const char *PyImath::V3sArray::name()   { return "V3sArray"; }
template <> const char *PyImath::V3iArray::name()   { return "V3iArray"; }
template <> const char *PyImath::V3i64Array::name() { return "V3i64Array"; }

using namespace boost::python;
using namespace IMATH_NAMESPACE;

template<> const char *Vec3Name<unsigned char>::value() { return "V3c"; }
template<> const char *Vec3Name<short>::value()         { return "V3s"; }
template<> const char *Vec3Name<int>::value()           { return "V3i"; }
template<> const char *Vec3Name<int64_t>::value()       { return "V3i64"; }

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec3<unsigned char> > register_Vec3<unsigned char>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec3<short> >         register_Vec3<short>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec3<int> >           register_Vec3<int>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec3<int64_t> >       register_Vec3<int64_t>();

}

