//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathBoxArrayImpl.h"
#include "PyImathExport.h"

namespace PyImath {
using namespace boost::python;

template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box2s> >   register_BoxArray<IMATH_NAMESPACE::V2s>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box2i> >   register_BoxArray<IMATH_NAMESPACE::V2i>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box2i64> > register_BoxArray<IMATH_NAMESPACE::V2i64>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box2f> >   register_BoxArray<IMATH_NAMESPACE::V2f>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box2d> >   register_BoxArray<IMATH_NAMESPACE::V2d>();

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box2s   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box2s>::value()   { return IMATH_NAMESPACE::Box2s(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box2i   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box2i>::value()   { return IMATH_NAMESPACE::Box2i(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box2i64 PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box2i64>::value() { return IMATH_NAMESPACE::Box2i64(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box2f   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box2f>::value()   { return IMATH_NAMESPACE::Box2f(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box2d   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box2d>::value()   { return IMATH_NAMESPACE::Box2d(); }
}
