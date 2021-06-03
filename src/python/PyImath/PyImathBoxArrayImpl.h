//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathBoxArrayImpl_h_
#define _PyImathBoxArrayImpl_h_

//
// This .C file was turned into a header file so that instantiations
// of the various Box* types can be spread across multiple files in
// order to work around MSVC limitations.
//

#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/format.hpp>
#include <ImathVec.h>
#include <ImathVecAlgo.h>
#include <ImathBox.h>
#include "PyImath.h"
#include "PyImathBox.h"
#include "PyImathDecorators.h"
#include "PyImathMathExc.h"
#include "PyImathOperators.h"
#include "PyImathVecOperators.h"

namespace PyImath {
using namespace boost::python;
using namespace IMATH_NAMESPACE;

template <class T,int index>
static FixedArray<T>
BoxArray_get(FixedArray<IMATH_NAMESPACE::Box<T> > &va)
{
    return index == 0 ? 
           FixedArray<T>(&(va.unchecked_index(0).min),
                         va.len(),2*va.stride(),va.handle(),va.writable()) :
           FixedArray<T>(&(va.unchecked_index(0).max),
                         va.len(),2*va.stride(),va.handle(),va.writable());
}

template <class T>
static void
setItemTuple(FixedArray<IMATH_NAMESPACE::Box<T> > &va, Py_ssize_t index, const tuple &t)
{
    if(t.attr("__len__")() == 2)
    {
        Box<T> v;
        v.min = extract<T>(t[0]);
        v.max = extract<T>(t[1]);
        va[(size_t)va.canonical_index(index)] = v;
    }
    else
      throw std::invalid_argument ("tuple of length 2 expected");
}

template <class T>
class_<FixedArray<IMATH_NAMESPACE::Box<T> > >
register_BoxArray()
{
    using boost::mpl::true_;
    using boost::mpl::false_;

    class_<FixedArray<IMATH_NAMESPACE::Box<T> > > boxArray_class = FixedArray<IMATH_NAMESPACE::Box<T> >::register_("Fixed length array of IMATH_NAMESPACE::Box");
    boxArray_class
        .add_property("min",&BoxArray_get<T,0>)
        .add_property("max",&BoxArray_get<T,1>)
        .def("__setitem__", &setItemTuple<T>)
    ;

    add_comparison_functions(boxArray_class);
    decoratecopy(boxArray_class);

    return boxArray_class;
}

}  // namespace PyImath

#endif   // _PyImathBoxArrayImpl_h_
