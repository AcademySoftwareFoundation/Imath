//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec4Impl.h"
#include "PyImathExport.h"

namespace PyImath {
template <> const char *PyImath::V4cArray::name()   { return "V4cArray"; }
template <> const char *PyImath::V4sArray::name()   { return "V4sArray"; }
template <> const char *PyImath::V4iArray::name()   { return "V4iArray"; }
template <> const char *PyImath::V4i64Array::name() { return "V4i64Array"; }

using namespace boost::python;
using namespace IMATH_NAMESPACE;

template<> const char *Vec4Name<unsigned char>::value() { return "V4c"; }
template<> const char *Vec4Name<short>::value()         { return "V4s"; }
template<> const char *Vec4Name<int>::value()           { return "V4i"; }
template<> const char *Vec4Name<int64_t>::value()       { return "V4i64"; }

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec4<unsigned char> > register_Vec4<unsigned char>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec4<short> >         register_Vec4<short>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec4<int> >           register_Vec4<int>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec4<int64_t> >       register_Vec4<int64_t>();

}

