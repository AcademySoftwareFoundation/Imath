//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathVec_h_
#define _PyImathVec_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathVec.h>
#include "PyImath.h"
#include "PyImathFixedArray.h"

namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Vec2<T> > register_Vec2();
template <class T> boost::python::class_<FixedArray<IMATH_NAMESPACE::Vec2<T> > > register_Vec2Array();
typedef FixedArray<IMATH_NAMESPACE::V2s>   V2sArray;
typedef FixedArray<IMATH_NAMESPACE::V2i>   V2iArray;
typedef FixedArray<IMATH_NAMESPACE::V2i64> V2i64Array;
typedef FixedArray<IMATH_NAMESPACE::V2f>   V2fArray;
typedef FixedArray<IMATH_NAMESPACE::V2d>   V2dArray;

// TODO: template <class T> class Vec2Array : public FixedArray<IMATH_NAMESPACE::Vec2<T> >

}

// define vector*float array multiplication
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > &a0, T v1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]*v1; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > operator * (T v0, const PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > &a1) { return a1*v0; }
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > &a0, const PyImath::FixedArray<T> &a1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.match_dimension(a1); PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]*a1[i]; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > operator * (const PyImath::FixedArray<T> &a0, const PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > &a1) {
    return a1*a0;
}

// define vector/float array division
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > operator / (const PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > &a0, T v1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]/v1; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > operator / (const PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > &a0, const PyImath::FixedArray<T> &a1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.match_dimension(a1); PyImath::FixedArray<IMATH_NAMESPACE::Vec2<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]/a1[i]; return f;
}


namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Vec3<T> > register_Vec3();
template <class T> boost::python::class_<FixedArray<IMATH_NAMESPACE::Vec3<T> > > register_Vec3Array();
typedef FixedArray<IMATH_NAMESPACE::Vec3<unsigned char> >  V3cArray;
typedef FixedArray<IMATH_NAMESPACE::V3s>   V3sArray;
typedef FixedArray<IMATH_NAMESPACE::V3i>   V3iArray;
typedef FixedArray<IMATH_NAMESPACE::V3i64> V3i64Array;
typedef FixedArray<IMATH_NAMESPACE::V3f>   V3fArray;
typedef FixedArray<IMATH_NAMESPACE::V3d>   V3dArray;

// TODO: template <class T> class Vec3Array : public FixedArray<IMATH_NAMESPACE::Vec3<T> >
}

// define vector*float array multiplication
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &a0, T v1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]*v1; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator * (T v0, const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &a1) { return a1*v0; }
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &a0, const PyImath::FixedArray<T> &a1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.match_dimension(a1); PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]*a1[i]; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator * (const PyImath::FixedArray<T> &a0, const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &a1) {
    return a1*a0;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &va, const IMATH_NAMESPACE::M44f &m) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = va.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > f(len); for (size_t i = 0; i < len; ++i) f[i] = va[i] * m; return f;
}

template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &va, const IMATH_NAMESPACE::M44d &m) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = va.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > f(len); for (size_t i = 0; i < len; ++i) f[i] = va[i] * m; return f;
}

// define vector/float array division
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator / (const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &a0, T v1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]/v1; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > operator / (const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > &a0, const PyImath::FixedArray<T> &a1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.match_dimension(a1); PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]/a1[i]; return f;
}

namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Vec4<T> > register_Vec4();
template <class T> boost::python::class_<PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > > register_Vec4Array();
typedef FixedArray<IMATH_NAMESPACE::Vec4<unsigned char> >  V4cArray;
typedef FixedArray<IMATH_NAMESPACE::V4s>   V4sArray;
typedef FixedArray<IMATH_NAMESPACE::V4i>   V4iArray;
typedef FixedArray<IMATH_NAMESPACE::V4i64> V4i64Array;
typedef FixedArray<IMATH_NAMESPACE::V4f>   V4fArray;
typedef FixedArray<IMATH_NAMESPACE::V4d>   V4dArray;

// TODO: template <class T> class Vec3Array : public FixedArray<IMATH_NAMESPACE::Vec3<T> >
}

// define vector*float array multiplication
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &a0, T v1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]*v1; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator * (T v0, const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &a1) { return a1*v0; }
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &a0, const PyImath::FixedArray<T> &a1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.match_dimension(a1); PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]*a1[i]; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator * (const PyImath::FixedArray<T> &a0, const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &a1) {
    return a1*a0;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &va, const IMATH_NAMESPACE::M44f &m) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = va.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > f(len); for (size_t i = 0; i < len; ++i) f[i] = va[i] * m; return f;
}

template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator * (const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &va, const IMATH_NAMESPACE::M44d &m) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = va.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > f(len); for (size_t i = 0; i < len; ++i) f[i] = va[i] * m; return f;
}

// define vector/float array division
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator / (const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &a0, T v1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.len(); PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]/v1; return f;
}
template <class T>
static PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > operator / (const PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > &a0, const PyImath::FixedArray<T> &a1) {
    PY_IMATH_LEAVE_PYTHON;
    size_t len = a0.match_dimension(a1); PyImath::FixedArray<IMATH_NAMESPACE::Vec4<T> > f(len); for (size_t i=0;i<len;++i) f[i]=a0[i]/a1[i]; return f;
}


//

namespace PyImath {

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type for the vector (e.g., int,
// float, double).

template <class T>
class V2 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Vec2<T> &v);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Vec2<T> *v);
};

template <class T>
class V3 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Vec3<T> &v);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Vec3<T> *v);
};

template <class T>
class V4 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Vec4<T> &v);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Vec4<T> *v);
};

template <class T>
PyObject *
V2<T>::wrap (const IMATH_NAMESPACE::Vec2<T> &v)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Vec2<T> >::type converter;
    PyObject *p = converter (v);
    return p;
}

template <class T>
PyObject *
V3<T>::wrap (const IMATH_NAMESPACE::Vec3<T> &v)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Vec3<T> >::type converter;
    PyObject *p = converter (v);
    return p;
}

template <class T>
PyObject *
V4<T>::wrap (const IMATH_NAMESPACE::Vec4<T> &v)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Vec4<T> >::type converter;
    PyObject *p = converter (v);
    return p;
}

template <class T>
int
V2<T>::convert (PyObject *p, IMATH_NAMESPACE::Vec2<T> *v)
{
    boost::python::extract <IMATH_NAMESPACE::V2i> extractorV2i (p);
    if (extractorV2i.check())
    {
        IMATH_NAMESPACE::V2i v2i = extractorV2i();
        v->setValue (T(v2i[0]), T(v2i[1]));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V2i64> extractorV2i64 (p);
    if (extractorV2i64.check())
    {
        IMATH_NAMESPACE::V2i64 v2i64 = extractorV2i64();
        v->setValue (T(v2i64[0]), T(v2i64[1]));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V2f> extractorV2f (p);
    if (extractorV2f.check())
    {
        IMATH_NAMESPACE::V2f v2f = extractorV2f();
        v->setValue (T(v2f[0]), T(v2f[1]));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V2d> extractorV2d (p);
    if (extractorV2d.check())
    {
        IMATH_NAMESPACE::V2d v2d = extractorV2d();
        v->setValue (T(v2d[0]), T(v2d[1]));
        return 1;
    }

    boost::python::extract <boost::python::tuple> extractorTuple (p);
    if (extractorTuple.check())
    {
        boost::python::tuple t = extractorTuple();
        if (t.attr ("__len__") () == 2)
        {
            // Extracting the tuple elements as doubles and casting them to
            // Ts in setValue() works better than extracting them as Ts from
            // the start.  Extracting them as Ts can fail in certain
            // circumstances if T is int and the tuples elemnts are floats.
            // In particular, this kind of failure occurs in PyImathBox.h,
            // when Box2<int>::convert() is passed a tuple of two tuples of
            // floats. 

            double a = boost::python::extract <double> (t[0]);
            double b = boost::python::extract <double> (t[1]);
            v->setValue (T(a), T(b));
            return 1;
        }
    }

    boost::python::extract <boost::python::list> extractorList (p);
    if (extractorList.check())
    {
        boost::python::list l = extractorList();
        if (l.attr ("__len__") () == 2)
        {
            boost::python::extract <double> extractor0 (l[0]);
            boost::python::extract <double> extractor1 (l[1]);
            if (extractor0.check() && extractor1.check())
            {
                v->setValue (T(extractor0()), T(extractor1()));
                return 1;
            }
        }
    }

    return 0;
}

template <class T>
int
V3<T>::convert (PyObject *p, IMATH_NAMESPACE::Vec3<T> *v)
{
    boost::python::extract <IMATH_NAMESPACE::V3i> extractorV3i (p);
    if (extractorV3i.check())
    {
        IMATH_NAMESPACE::V3i v3i = extractorV3i();
        v->setValue (T(v3i[0]), T(v3i[1]), T(v3i[2]));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V3i64> extractorV3i64 (p);
    if (extractorV3i64.check())
    {
        IMATH_NAMESPACE::V3i64 v3i64 = extractorV3i64();
        v->setValue (T(v3i64[0]), T(v3i64[1]), T(v3i64[2]));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V3f> extractorV3f (p);
    if (extractorV3f.check())
    {
        IMATH_NAMESPACE::V3f v3f = extractorV3f();
        v->setValue (T(v3f[0]), T(v3f[1]), T(v3f[2]));
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V3d> extractorV3d (p);
    if (extractorV3d.check())
    {
        IMATH_NAMESPACE::V3d v3d = extractorV3d();
        v->setValue (T(v3d[0]), T(v3d[1]), T(v3d[2]));
        return 1;
    }

    boost::python::extract <boost::python::tuple> extractorTuple (p);
    if (extractorTuple.check())
    {
        boost::python::tuple t = extractorTuple();
        if (t.attr ("__len__") () == 3)
        {
            // See the comment in V2<T>::convert().

            double a = boost::python::extract <double> (t[0]);
            double b = boost::python::extract <double> (t[1]);
            double c = boost::python::extract <double> (t[2]);
            v->setValue (T(a), T(b), T(c));
            return 1;
        }
    }

    boost::python::extract <boost::python::list> extractorList (p);
    if (extractorList.check())
    {
        boost::python::list l = extractorList();
        if (l.attr ("__len__") () == 3)
        {
            boost::python::extract <double> extractor0 (l[0]);
            boost::python::extract <double> extractor1 (l[1]);
            boost::python::extract <double> extractor2 (l[2]);
            if (extractor0.check() && extractor1.check() &&
                extractor2.check())
            {
                v->setValue (T(extractor0()), T(extractor1()),
                             T(extractor2()));
                return 1;
            }
        }
    }

    return 0;
}

template <class T>
int
V4<T>::convert (PyObject *p, IMATH_NAMESPACE::Vec4<T> *v)
{
    boost::python::extract <IMATH_NAMESPACE::V4i> extractorV4i (p);
    if (extractorV4i.check())
    {
        IMATH_NAMESPACE::V4i v4i = extractorV4i();
        *v = IMATH_NAMESPACE::Vec4<T>(v4i);
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V4f> extractorV4f (p);
    if (extractorV4f.check())
    {
        IMATH_NAMESPACE::V4f v4f = extractorV4f();
        *v = IMATH_NAMESPACE::Vec4<T>(v4f);
        return 1;
    }

    boost::python::extract <IMATH_NAMESPACE::V4d> extractorV4d (p);
    if (extractorV4d.check())
    {
        IMATH_NAMESPACE::V4d v4d = extractorV4d();
        *v = IMATH_NAMESPACE::Vec4<T>(v4d);
        return 1;
    }

    boost::python::extract <boost::python::tuple> extractorTuple (p);
    if (extractorTuple.check())
    {
        boost::python::tuple t = extractorTuple();
        if (t.attr ("__len__") () == 4)
        {
            // See the comment in V2<T>::convert().

            double a = boost::python::extract <double> (t[0]);
            double b = boost::python::extract <double> (t[1]);
            double c = boost::python::extract <double> (t[2]);
            double d = boost::python::extract <double> (t[3]);
            *v = IMATH_NAMESPACE::Vec4<T>(T(a), T(b), T(c), T(d));
            return 1;
        }
    }

    boost::python::extract <boost::python::list> extractorList (p);
    if (extractorList.check())
    {
        boost::python::list l = extractorList();
        if (l.attr ("__len__") () == 4)
        {
            boost::python::extract <double> extractor0 (l[0]);
            boost::python::extract <double> extractor1 (l[1]);
            boost::python::extract <double> extractor2 (l[2]);
            boost::python::extract <double> extractor3 (l[3]);
            if (extractor0.check() && extractor1.check() &&
                extractor2.check() && extractor3.check())
            {
                *v = IMATH_NAMESPACE::Vec4<T>(T(extractor0()), T(extractor1()),
                             T(extractor2()), T(extractor3()));
                return 1;
            }
        }
    }

    return 0;
}


typedef V2<int>		V2i;
typedef V2<float>	V2f;
typedef V2<double>	V2d;
typedef V3<int>		V3i;
typedef V3<float>	V3f;
typedef V3<double>	V3d;
typedef V4<int>		V4i;
typedef V4<float>	V4f;
typedef V4<double>	V4d;

}

#endif
