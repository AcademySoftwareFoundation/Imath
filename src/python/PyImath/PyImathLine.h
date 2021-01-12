//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathLine_h_
#define _PyImathLine_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathLine.h>
#include "PyImath.h"


namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Line3<T> > register_Line();

//

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type (e.g.,float, double).

template <class T>
class L3 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Line3<T> &l);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Line3<T> *l);
};

template <class T>
PyObject *
L3<T>::wrap (const IMATH_NAMESPACE::Line3<T> &l)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Line3<T> >::type converter;
    PyObject *p = converter (l);
    return p;
}

template <class T>
int
L3<T>::convert (PyObject *p, IMATH_NAMESPACE::Line3<T> *l)
{
    boost::python::extract <IMATH_NAMESPACE::Line3f> extractorLf (p);
    if (extractorLf.check())
    {
        IMATH_NAMESPACE::Line3f e = extractorLf();
        l->pos.setValue (e.pos);
        l->dir.setValue (e.dir);
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::Line3d> extractorLd (p);
    if (extractorLd.check())
    {
        IMATH_NAMESPACE::Line3d e = extractorLd();
        l->pos.setValue (e.pos);
        l->dir.setValue (e.dir);
        return 1;
    }

    return 0;
}

typedef L3<float>	Line3f;
typedef L3<double>	Line3d;

}

#endif
