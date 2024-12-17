//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathVec.h>
#include <ImathVecAlgo.h>

namespace PyBindImath {

template <class T>
void register_vec(pybind11::class_<T>& c)
{
    c.def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def(pybind11::self + pybind11::self)
    .def(pybind11::self += pybind11::self)
    .def(pybind11::self * pybind11::self)
    .def(pybind11::self * int())
    .def(pybind11::self * float())
    .def(pybind11::self * double())
    .def(pybind11::self *= pybind11::self)
    .def(pybind11::self *= int())
    .def(pybind11::self *= float())
    .def(pybind11::self *= double())
    .def(pybind11::self / pybind11::self)
    .def(pybind11::self / int())
    .def(pybind11::self / float())
    .def(pybind11::self / double())
    .def(pybind11::self /= pybind11::self)
    .def(pybind11::self /= int())
    .def(pybind11::self /= float())
    .def(pybind11::self /= double())
    .def(pybind11::self - pybind11::self)
    .def(pybind11::self -= pybind11::self)
    .def(pybind11::self ^ pybind11::self)
    .def(-pybind11::self)
    .def_static("baseTypeEpsilon", &T::baseTypeEpsilon,"baseTypeEpsilon() epsilon value of the base type of the vector")
    .def_static("baseTypeMax", &T::baseTypeMax,"baseTypeMax() max value of the base type of the vector")
    .def_static("baseTypeLowest", &T::baseTypeLowest,"baseTypeLowest() largest negative value of the base type of the vector")
    .def_static("baseTypeSmallest", &T::baseTypeSmallest,"baseTypeSmallest() smallest value of the base type of the vector")
    .def("dot", &T::dot, "v1.dot(v2) inner product of the two vectors")
    .def("dimensions", &T::dimensions, "dimensions() number of dimensions in the vector")
    .def("equalWithAbsError", &T::equalWithAbsError, "v1.equalWithAbsError(v2) true if the elements of v1 and v2 are the same with an absolute error of no more than e, i.e., abs(v1[i] - v2[i]) <= e")
    .def("equalWithRelError", &T::equalWithRelError, "v1.equalWithRelError(v2) true if the elements of v1 and v2 are the same with a relative error of no more than e, i.e., abs(v1[i] - v2[i]) <= e * abs(v1[i])")
    .def("length", &T::length, "length() magnitude of the vector")
    .def("length2", &T::length2,"length2() square magnitude of the vector")
    .def("negate", &T::negate)
    .def("normalize", &T::normalize, "v.normalize() destructively normalizes v and returns a reference to it")
    .def("normalizeExc", &T::normalizeExc, "v.normalizeExc() destructively normalizes V and returns a reference to it, throwing an exception if length() == 0")
    .def("normalizeNonNull",  &T::normalizeNonNull, "v.normalizeNonNull() destructively normalizes V and returns a reference to it, faster if length() != 0")
    .def("normalized", &T::normalized, "v.normalized() returns a normalized copy of v")
    .def("normalizedExc", &T::normalizedExc, "v.normalizedExc() returns a normalized copy of v, throwing an exception if length() == 0")
    .def("normalizedNonNull", &T::normalizedNonNull, "v.normalizedNonNull() returns a normalized copy of v, faster if lngth() != 0")

    // things from ImathVecAlgo
    .def("closestVertex", &IMATH_NAMESPACE::closestVertex<T>)
    .def("orthogonal", &IMATH_NAMESPACE::orthogonal<T>)
    .def("project", &IMATH_NAMESPACE::project<T>)
    .def("reflect", &IMATH_NAMESPACE::reflect<T>)
    ;
}

template <class T, class S>
void register_vec2(pybind11::module& m, const char * name)
{
    // no %=

    pybind11::class_<T> c(m, name);
    c.def(pybind11::init<>())
    .def(pybind11::init<S>())
    .def(pybind11::init<S, S>())
    .def(pybind11::self % pybind11::self)
    .def("cross", &T::cross, "v1.cross(v2) right handed cross product")
    .def_readwrite("x", &T::x)
    .def_readwrite("y", &T::y);

    register_vec<T>(c);
}

template <class T, class S>
void register_vec3(pybind11::module& m, const char * name)
{
    pybind11::class_<T> c(m, name);
    c.def(pybind11::init<>())
    .def(pybind11::init<S>())
    .def(pybind11::init<S, S, S>())
    .def(pybind11::self % pybind11::self)
    .def(pybind11::self %= pybind11::self)
    .def("cross", &T::cross, "v1.cross(v2) right handed cross product")
    .def_readwrite("x", &T::x)
    .def_readwrite("y", &T::y)
    .def_readwrite("z", &T::z);

    register_vec<T>(c);
}

template <class T, class S>
void register_vec4(pybind11::module& m, const char * name)
{
    // Does a cross product not exist for Vec4? if not then thats why there is no % %= cross
    pybind11::class_<T> c(m, name);
    c.def(pybind11::init<>())
    .def(pybind11::init<S>())
    .def(pybind11::init<S, S, S, S>())
    .def_readwrite("x", &T::x)
    .def_readwrite("y", &T::y)
    .def_readwrite("z", &T::z)
    .def_readwrite("w", &T::z);

    register_vec<T>(c);
}

void register_imath_vec(pybind11::module& m)
{
    register_vec2<IMATH_NAMESPACE::V2d, double>(m, "V2d");
    register_vec2<IMATH_NAMESPACE::V2f, float>(m, "V2f");

    register_vec3<IMATH_NAMESPACE::V3d, double>(m, "V3d");
    register_vec3<IMATH_NAMESPACE::V3f, float>(m, "V3f");

    register_vec4<IMATH_NAMESPACE::V4d, double>(m, "V4d");
    register_vec4<IMATH_NAMESPACE::V4f, float>(m, "V4f");
}

}
