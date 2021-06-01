//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathColor4ArrayImpl_h_
#define _PyImathColor4ArrayImpl_h_

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
Color4Array_get(FixedArray<IMATH_NAMESPACE::Color4<T> > &ca)
{    
    return FixedArray<T>(&(ca.unchecked_index(0)[index]),
                         ca.len(),4*ca.stride(),ca.handle(),ca.writable());
}

// Currently we are only exposing the RGBA components.
template <class T>
class_<FixedArray<IMATH_NAMESPACE::Color4<T> > >
register_Color4Array()
{
    class_<FixedArray<IMATH_NAMESPACE::Color4<T> > > color4Array_class = FixedArray<IMATH_NAMESPACE::Color4<T> >::register_("Fixed length array of IMATH_NAMESPACE::Color4");
    color4Array_class
        .add_property("r",&Color4Array_get<T,0>)
        .add_property("g",&Color4Array_get<T,1>)
        .add_property("b",&Color4Array_get<T,2>)
        .add_property("a",&Color4Array_get<T,3>)
        ;

    return color4Array_class;
}

} // namespace PyImath

#endif // _PyImathColor4ArrayImpl_h_
