//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathFrustum_h_
#define _PyImathFrustum_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathFrustum.h>
#include <ImathFrustumTest.h>
#include "PyImath.h"


namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Frustum<T> > register_Frustum();
template <class T> boost::python::class_<IMATH_NAMESPACE::FrustumTest<T> > register_FrustumTest();

//

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type (e.g.,float, double).

template <class T>
class F {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Frustum<T> &f);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Frustum<T> *f);
};

template <class T>
PyObject *
F<T>::wrap (const IMATH_NAMESPACE::Frustum<T> &f)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Frustum<T> >::type converter;
    PyObject *p = converter (f);
    return p;
}

template <class T>
int
F<T>::convert (PyObject *p, IMATH_NAMESPACE::Frustum<T> *f)
{
    boost::python::extract <IMATH_NAMESPACE::Frustumf> extractorEf (p);
    if (extractorEf.check())
    {
        IMATH_NAMESPACE::Frustumf e = extractorEf();
        f->set (T(e.nearPlane()), T(e.farPlane()), T(e.left()), T(e.right()),
                T(e.top()), T(e.bottom()), e.orthographic());
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::Frustumd> extractorEd (p);
    if (extractorEd.check())
    {
        IMATH_NAMESPACE::Frustumd e = extractorEd();
        f->set (T(e.nearPlane()), T(e.farPlane()), T(e.left()), T(e.right()),
                T(e.top()), T(e.bottom()), e.orthographic());
        return 1;
    }

    return 0;
}

typedef F<float>	Frustumf;
typedef F<double>	Frustumd;

}

#endif
