//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathBoxArrayImpl.h"
#include "PyImathExport.h"

namespace PyImath {
using namespace boost::python;

template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box3s> >   register_BoxArray<IMATH_NAMESPACE::V3s>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box3i> >   register_BoxArray<IMATH_NAMESPACE::V3i>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box3i64> > register_BoxArray<IMATH_NAMESPACE::V3i64>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box3f> >   register_BoxArray<IMATH_NAMESPACE::V3f>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Box3d> >   register_BoxArray<IMATH_NAMESPACE::V3d>();

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box3s   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box3s>::value()   { return IMATH_NAMESPACE::Box3s(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box3i   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box3i>::value()   { return IMATH_NAMESPACE::Box3i(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box3i64 PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box3i64>::value() { return IMATH_NAMESPACE::Box3i64(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box3f   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box3f>::value()   { return IMATH_NAMESPACE::Box3f(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Box3d   PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Box3d>::value()   { return IMATH_NAMESPACE::Box3d(); }
}
