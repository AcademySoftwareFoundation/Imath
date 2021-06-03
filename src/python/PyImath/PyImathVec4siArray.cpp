//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathVec4ArrayImpl.h"
#include "PyImathExport.h"

namespace PyImath {
using namespace boost::python;
using namespace IMATH_NAMESPACE;

template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec4<unsigned char> > > register_Vec4Array<unsigned char>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec4<short> > >         register_Vec4Array<short>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec4<int> > >           register_Vec4Array<int>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Vec4<int64_t> > >       register_Vec4Array<int64_t>();

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec4<unsigned char> FixedArrayDefaultValue<IMATH_NAMESPACE::Vec4<unsigned char> >::value() { return IMATH_NAMESPACE::Vec4<unsigned char>(0,0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec4<short>         FixedArrayDefaultValue<IMATH_NAMESPACE::Vec4<short> >::value()         { return IMATH_NAMESPACE::Vec4<short>(0,0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec4<int>           FixedArrayDefaultValue<IMATH_NAMESPACE::Vec4<int> >::value()           { return IMATH_NAMESPACE::Vec4<int>(0,0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Vec4<int64_t>       FixedArrayDefaultValue<IMATH_NAMESPACE::Vec4<int64_t> >::value()       { return IMATH_NAMESPACE::Vec4<int64_t>(0,0,0,0); }
}
