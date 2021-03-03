//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathBox_h_
#define _PyImathBox_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathBox.h>
#include "PyImathVec.h"
#include "PyImathFixedArray.h"

namespace PyImath {

template <class T> boost::python::class_<IMATH_NAMESPACE::Box<T> > register_Box2();
template <class T> boost::python::class_<IMATH_NAMESPACE::Box<T> > register_Box3();

template <class T> boost::python::class_<FixedArray<IMATH_NAMESPACE::Box<T> > > register_BoxArray();

typedef FixedArray<IMATH_NAMESPACE::Box2s>   Box2sArray;
typedef FixedArray<IMATH_NAMESPACE::Box2i>   Box2iArray;
typedef FixedArray<IMATH_NAMESPACE::Box2i64> Box2i64Array;
typedef FixedArray<IMATH_NAMESPACE::Box2f>   Box2fArray;
typedef FixedArray<IMATH_NAMESPACE::Box2d>   Box2dArray;

typedef FixedArray<IMATH_NAMESPACE::Box3s>   Box3sArray;
typedef FixedArray<IMATH_NAMESPACE::Box3i>   Box3iArray;
typedef FixedArray<IMATH_NAMESPACE::Box3i64> Box3i64Array;
typedef FixedArray<IMATH_NAMESPACE::Box3f>   Box3fArray;
typedef FixedArray<IMATH_NAMESPACE::Box3d>   Box3dArray;

//

// Other code in the Zeno code base assumes the existance of a class with the
// same name as the Imath class, and with static functions wrap() and
// convert() to produce a PyImath object from an Imath object and vice-versa,
// respectively.  The class Boost generates from the Imath class does not
// have these properties, so we define a companion class here.
// The template argument, T, is the element type for the box (e.g., int,
// float).

template <class T>
class Box2 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec2<T> > &b);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec2<T> > *b);
};

template <class T>
class Box3 {
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec3<T> > &b);
    static int		convert (PyObject *p, IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec3<T> > *v);
};

template <class T>
PyObject *
Box2<T>::wrap (const IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec2<T> > &b)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec2<T> > >::type converter;
    PyObject *p = converter (b);
    return p;
}

template <class T>
PyObject *
Box3<T>::wrap (const IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec3<T> > &b)
{
    typename boost::python::return_by_value::apply < IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec3<T> > >::type converter;
    PyObject *p = converter (b);
    return p;
}

template <class T>
int
Box2<T>::convert (PyObject *p, IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec2<T> > *v)
{
    boost::python::extract < IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V2i> > extractorV2i (p);
    if (extractorV2i.check())
    {
        IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V2i> b = extractorV2i();
        v->min = b.min;
        v->max = b.max;
        return 1;
    }

    boost::python::extract < IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V2f> > extractorV2f (p);
    if (extractorV2f.check())
    {
        IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V2f> b = extractorV2f();
        v->min = b.min;
        v->max = b.max;
        return 1;
    }

    boost::python::extract < IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V2d> > extractorV2d (p);
    if (extractorV2d.check())
    {
        IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V2d> b = extractorV2d();
        v->min = b.min;
        v->max = b.max;
        return 1;
    }

    boost::python::extract <boost::python::tuple> extractorTuple (p);
    if (extractorTuple.check())
    {
        boost::python::tuple t = extractorTuple();
        if (t.attr ("__len__") () == 2)
        {
            PyObject *minObj = 
                boost::python::extract <boost::python::object> (t[0])().ptr();
            PyObject *maxObj = 
                boost::python::extract <boost::python::object> (t[1])().ptr();

            IMATH_NAMESPACE::Vec2<T> min, max;
            if (! V2<T>::convert (minObj, &min))
                return 0;
            if (! V2<T>::convert (maxObj, &max))
                return 0;

            v->min = min;
            v->max = max;

            return 1;
        }
    }

    return 0;
}

template <class T>
int
Box3<T>::convert (PyObject *p, IMATH_NAMESPACE::Box< IMATH_NAMESPACE::Vec3<T> > *v)
{
    boost::python::extract < IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V3i> > extractorV3i (p);
    if (extractorV3i.check())
    {
        IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V3i> b = extractorV3i();
        v->min = b.min;
        v->max = b.max;
        return 1;
    }

    boost::python::extract < IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V3f> > extractorV3f (p);
    if (extractorV3f.check())
    {
        IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V3f> b = extractorV3f();
        v->min = b.min;
        v->max = b.max;
        return 1;
    }

    boost::python::extract < IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V3d> > extractorV3d (p);
    if (extractorV3d.check())
    {
        IMATH_NAMESPACE::Box<IMATH_NAMESPACE::V3d> b = extractorV3d();
        v->min = b.min;
        v->max = b.max;
        return 1;
    }

    boost::python::extract <boost::python::tuple> extractorTuple (p);
    if (extractorTuple.check())
    {
        boost::python::tuple t = extractorTuple();
        if (t.attr ("__len__") () == 2)
        {
            PyObject *minObj = 
                boost::python::extract <boost::python::object> (t[0])().ptr();
            PyObject *maxObj = 
                boost::python::extract <boost::python::object> (t[1])().ptr();

            IMATH_NAMESPACE::Vec3<T> min, max;
            if (! V3<T>::convert (minObj, &min))
                return 0;
            if (! V3<T>::convert (maxObj, &max))
                return 0;

            v->min = min;
            v->max = max;

            return 1;
        }
    }

    return 0;
}

typedef Box2<int>	Box2i;
typedef Box2<int64_t>	Box2i64;
typedef Box2<float>	Box2f;
typedef Box2<double>	Box2d;

typedef Box3<int>	Box3i;
typedef Box3<int64_t>	Box3i64;
typedef Box3<float>	Box3f;
typedef Box3<double>	Box3d;

}

#endif
