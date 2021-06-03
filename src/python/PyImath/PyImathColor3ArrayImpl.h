//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathColor3ArrayImpl_h_
#define _PyImathColor3ArrayImpl_h_

//
// This .C file was turned into a header file so that instantiations
// of the various V3* types can be spread across multiple files in
// order to work around MSVC limitations.
//

#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/format.hpp>
#include "PyImath.h"
#include "PyImathMathExc.h"
#include "PyImathDecorators.h"

namespace PyImath {
using namespace boost::python;
using namespace IMATH_NAMESPACE;

// XXX fixme - template this
// really this should get generated automatically...

template <class T,int index>
static FixedArray<T>
Color3Array_get(FixedArray<IMATH_NAMESPACE::Color3<T> > &ca)
{    
    return FixedArray<T>(&(ca.unchecked_index(0)[index]),
                         ca.len(),3*ca.stride(),ca.handle(),ca.writable());
}

// Currently we are only exposing the RGBA components.
template <class T>
class_<FixedArray<IMATH_NAMESPACE::Color3<T> > >
register_Color3Array()
{
    class_<FixedArray<IMATH_NAMESPACE::Color3<T> > > color3Array_class = FixedArray<IMATH_NAMESPACE::Color3<T> >::register_("Fixed length array of Imath::Color3");
    color3Array_class
        .add_property("r",&Color3Array_get<T,0>)
        .add_property("g",&Color3Array_get<T,1>)
        .add_property("b",&Color3Array_get<T,2>)
        ;

    return color3Array_class;
}

} // namespace PyImath

#endif // _PyImathColor3ArrayImpl_h_
