//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathEuler_h_
#define _PyImathEuler_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathEuler.h>
#include <ImathVec.h>
#include "PyImath.h"

namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Euler<T>,boost::python::bases<IMATH_NAMESPACE::Vec3<T> > > register_Euler();
template <class T> boost::python::class_<PyImath::FixedArray<IMATH_NAMESPACE::Euler<T> > > register_EulerArray();
typedef FixedArray<IMATH_NAMESPACE::Eulerf>  EulerfArray;
typedef FixedArray<IMATH_NAMESPACE::Eulerd>  EulerdArray;

//

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type for the axis vector 
// (e.g.,float, double).

template <class T>
class E {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Euler<T> &e);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Euler<T> *v);
};

template <class T>
PyObject *
E<T>::wrap (const IMATH_NAMESPACE::Euler<T> &e)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Euler<T> >::type converter;
    PyObject *p = converter (e);
    return p;
}

template <class T>
int
E<T>::convert (PyObject *p, IMATH_NAMESPACE::Euler<T> *v)
{
    boost::python::extract <IMATH_NAMESPACE::Eulerf> extractorEf (p);
    if (extractorEf.check())
    {
        IMATH_NAMESPACE::Eulerf e = extractorEf();
        v->x = T(e.x);
        v->y = T(e.y);
        v->z = T(e.z);
        v->setOrder (typename IMATH_NAMESPACE::Euler<T>::Order (e.order()));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::Eulerd> extractorEd (p);
    if (extractorEd.check())
    {
        IMATH_NAMESPACE::Eulerd e = extractorEd();
        v->x = T(e.x);
        v->y = T(e.y);
        v->z = T(e.z);
        v->setOrder (typename IMATH_NAMESPACE::Euler<T>::Order (e.order()));
        return 1;
    }

    return 0;
}

typedef E<float>	Eulerf;
typedef E<double>	Eulerd;

}

#endif
