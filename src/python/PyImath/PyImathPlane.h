//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathPlane_h_
#define _PyImathPlane_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathPlane.h>
#include "PyImath.h"


namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Plane3<T> > register_Plane();

//

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type (e.g.,float, double).

template <class T>
class P3 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Plane3<T> &pl);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Plane3<T> *pl);
};

template <class T>
PyObject *
P3<T>::wrap (const IMATH_NAMESPACE::Plane3<T> &pl)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Plane3<T> >::type converter;
    PyObject *p = converter (pl);
    return p;
}

template <class T>
int
P3<T>::convert (PyObject *p, IMATH_NAMESPACE::Plane3<T> *pl)
{
    boost::python::extract <IMATH_NAMESPACE::Plane3f> extractorPf (p);
    if (extractorPf.check())
    {
        IMATH_NAMESPACE::Plane3f e = extractorPf();
        pl->normal.setValue (e.normal);
        pl->distance = T(e.distance);
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::Plane3d> extractorPd (p);
    if (extractorPd.check())
    {
        IMATH_NAMESPACE::Plane3d e = extractorPd();
        pl->normal.setValue (e.normal);
        pl->distance = T(e.distance);
        return 1;
    }

    return 0;
}

typedef P3<float>	Plane3f;
typedef P3<double>	Plane3d;

}

#endif
