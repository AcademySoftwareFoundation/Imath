//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec2Impl.h"
#include "PyImathExport.h"

namespace PyImath {
template <> const char *PyImath::V2sArray::name()   { return "V2sArray"; }
template <> const char *PyImath::V2iArray::name()   { return "V2iArray"; }
template <> const char *PyImath::V2i64Array::name() { return "V2i64Array"; }

using namespace boost::python;
using namespace IMATH_NAMESPACE;

template<> const char *Vec2Name<short>::value   = "V2s";
template<> const char *Vec2Name<int>::value     = "V2i";
template<> const char *Vec2Name<int64_t>::value = "V2i64";

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec2<short> >   register_Vec2<short>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec2<int> >     register_Vec2<int>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Vec2<int64_t> > register_Vec2<int64_t>();
		 
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec2<short> > >   register_Vec2Array<short>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec2<int> > >     register_Vec2Array<int>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec2<int64_t> > > register_Vec2Array<int64_t>();

template<> IMATH_NAMESPACE::Vec2<short>   PYIMATH_EXPORT FixedArrayDefaultValue<IMATH_NAMESPACE::Vec2<short> >::value()   { return IMATH_NAMESPACE::Vec2<short>(0,0); }
template<> IMATH_NAMESPACE::Vec2<int>     PYIMATH_EXPORT FixedArrayDefaultValue<IMATH_NAMESPACE::Vec2<int> >::value()     { return IMATH_NAMESPACE::Vec2<int>(0,0); }
template<> IMATH_NAMESPACE::Vec2<int64_t> PYIMATH_EXPORT FixedArrayDefaultValue<IMATH_NAMESPACE::Vec2<int64_t> >::value() { return IMATH_NAMESPACE::Vec2<int64_t>(0,0); }
}
