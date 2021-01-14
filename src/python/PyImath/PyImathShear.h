//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathShear_h_
#define _PyImathShear_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathShear.h>


namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Shear6<T> > register_Shear();

//

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type (e.g.,float, double).

template <class T>
class S6 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Shear6<T> &s);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Shear6<T> *s);
};

template <class T>
PyObject *
S6<T>::wrap (const IMATH_NAMESPACE::Shear6<T> &s)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Shear6<T> >::type converter;
    PyObject *p = converter (s);
    return p;
}

template <class T>
int
S6<T>::convert (PyObject *p, IMATH_NAMESPACE::Shear6<T> *s)
{
    boost::python::extract <IMATH_NAMESPACE::Shear6f> extractorShear6f (p);
    if (extractorShear6f.check())
    {
        IMATH_NAMESPACE::Shear6f s6f = extractorShear6f();
	float xy, xz, yz, yx, zx, zy;
	s6f.getValue (xy, xz, yz, yx, zx, zy);
	s->setValue(T(xy), T(xz), T(yz), T(yx), T(zx), T(zy));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::Shear6d> extractorShear6d (p);
    if (extractorShear6d.check())
    {
        IMATH_NAMESPACE::Shear6d s6d = extractorShear6d();
	double xy, xz, yz, yx, zx, zy;
	s6d.getValue (xy, xz, yz, yx, zx, zy);
	s->setValue(T(xy), T(xz), T(yz), T(yx), T(zx), T(zy));
        return 1;
    }

    boost::python::extract <boost::python::tuple> extractorTuple (p);
    if (extractorTuple.check())
    {
        boost::python::tuple t = extractorTuple();
        if (t.attr ("__len__") () == 3)
        {
            double xy = boost::python::extract <double> (t[0]);
            double xz = boost::python::extract <double> (t[1]);
            double yz = boost::python::extract <double> (t[2]);
            s->setValue (T(xy), T(xz), T(yz), T(0), T(0), T(0));
            return 1;
        }

        else if (t.attr ("__len__") () == 6)
        {
            double xy = boost::python::extract <double> (t[0]);
            double xz = boost::python::extract <double> (t[1]);
            double yz = boost::python::extract <double> (t[2]);
            double yx = boost::python::extract <double> (t[3]);
            double zx = boost::python::extract <double> (t[4]);
            double zy = boost::python::extract <double> (t[5]);
            s->setValue (T(xy), T(xz), T(yz), T(yx), T(zx), T(zy));
            return 1;
        }
    }

    boost::python::extract <IMATH_NAMESPACE::V3i> extractorV3i (p);
    if (extractorV3i.check())
    {
        IMATH_NAMESPACE::V3i v3i = extractorV3i();
        s->setValue (T(v3i[0]), T(v3i[1]), T(v3i[2]), T(0), T(0), T(0));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V3f> extractorV3f (p);
    if (extractorV3f.check())
    {
        IMATH_NAMESPACE::V3f v3f = extractorV3f();
        s->setValue (T(v3f[0]), T(v3f[1]), T(v3f[2]), T(0), T(0), T(0));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V3d> extractorV3d (p);
    if (extractorV3d.check())
    {
        IMATH_NAMESPACE::V3d v3d = extractorV3d();
        s->setValue (T(v3d[0]), T(v3d[1]), T(v3d[2]), T(0), T(0), T(0));
        return 1;
    }

    return 0;
}

typedef S6<float>	Shear6f;
typedef S6<double>	Shear6d;

}

#endif
