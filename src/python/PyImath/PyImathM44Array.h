//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathM44Array_h_
#define _PyImathM44Array_h_

#include <boost/python.hpp>
#include <ImathMatrix.h>
#include "PyImathOperators.h"

namespace PyImath {
using namespace boost::python;


template <class T> struct M44ArrayName { static const char *value(); };

template <class T>
static void
setM44ArrayItem(FixedArray<IMATH_NAMESPACE::Matrix44<T> > &ma,
                Py_ssize_t index,
                const IMATH_NAMESPACE::Matrix44<T> &m)
{
    ma[ma.canonical_index(index)] = m;
}

template <class T>
class_<FixedArray<IMATH_NAMESPACE::Matrix44<T> > >
register_M44Array()
{
    class_<FixedArray<IMATH_NAMESPACE::Matrix44<T> > > m44Array_class = FixedArray<IMATH_NAMESPACE::Matrix44<T> >::register_("Fixed length array of IMATH_NAMESPACE::M44");
    m44Array_class
    .def("__setitem__", &setM44ArrayItem<T>)
    ;

    return m44Array_class;
}


}  // namespace PyImath

#endif   // _PyImathM44Array_h_
