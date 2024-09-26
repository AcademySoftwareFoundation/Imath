//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "NanobindImath.h"
#include <ImathVec.h>
#include <ImathVecAlgo.h>

namespace NanobindImath {

template <class T>
void register_vec(nanobind::class_<T>& c)
{
    c.def(nanobind::self == nanobind::self)
    .def(nanobind::self != nanobind::self)
    .def(nanobind::self + nanobind::self)
    .def(nanobind::self += nanobind::self)
    .def(nanobind::self * nanobind::self)
    .def(nanobind::self * int())
    .def(nanobind::self * float())
    .def(nanobind::self * double())
    .def(nanobind::self *= nanobind::self)
    .def(nanobind::self *= int())
    .def(nanobind::self *= float())
    .def(nanobind::self *= double())
    .def(nanobind::self / nanobind::self)
    .def(nanobind::self / int())
    .def(nanobind::self / float())
    .def(nanobind::self / double())
    .def(nanobind::self /= nanobind::self)
    .def(nanobind::self /= int())
    .def(nanobind::self /= float())
    .def(nanobind::self /= double())
    .def(nanobind::self - nanobind::self)
    .def(nanobind::self -= nanobind::self)
    .def(nanobind::self ^ nanobind::self)
    .def(-nanobind::self)
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
void register_vec2(nanobind::module_& m, const char * name)
{
    // no %=

    nanobind::class_<T> c(m, name);
    c.def(nanobind::init<>())
    .def(nanobind::init<S>())
    .def(nanobind::init<S, S>())
    .def(nanobind::self % nanobind::self)
    .def("cross", &T::cross, "v1.cross(v2) right handed cross product")
    .def_rw("x", &T::x)
    .def_rw("y", &T::y);

    register_vec<T>(c);
}

template <class T, class S>
void register_vec3(nanobind::module_& m, const char * name)
{
    nanobind::class_<T> c(m, name);
    c.def(nanobind::init<>())
    .def(nanobind::init<S>())
    .def(nanobind::init<S, S, S>())
    .def(nanobind::self % nanobind::self)
    .def(nanobind::self %= nanobind::self)
    .def("cross", &T::cross, "v1.cross(v2) right handed cross product")
    .def_rw("x", &T::x)
    .def_rw("y", &T::y)
    .def_rw("z", &T::z);

    register_vec<T>(c);
}

template <class T, class S>
void register_vec4(nanobind::module_& m, const char * name)
{
    // Does a cross product not exist for Vec4? if not then thats why there is no % %= cross
    nanobind::class_<T> c(m, name);
    c.def(nanobind::init<>())
    .def(nanobind::init<S>())
    .def(nanobind::init<S, S, S, S>())
    .def_rw("x", &T::x)
    .def_rw("y", &T::y)
    .def_rw("z", &T::z)
    .def_rw("w", &T::z);

    register_vec<T>(c);
}

void register_imath_vec(nanobind::module_& m)
{
    register_vec2<Imath::V2d, double>(m, "V2d");
    register_vec2<Imath::V2f, float>(m, "V2f");

    register_vec3<Imath::V3d, double>(m, "V3d");
    register_vec3<Imath::V3f, float>(m, "V3f");

    register_vec4<Imath::V4d, double>(m, "V4d");
    register_vec4<Imath::V4f, float>(m, "V4f");
}

}
