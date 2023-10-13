//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyImath.h"
#include <ImathVec.h>

namespace PyImath {

template <class T>
void register_vec(pybind11::class_<T>& c)
{
	c.def_static("baseTypeEpsilon", &T::baseTypeEpsilon,"baseTypeEpsilon() epsilon value of the base type of the vector")
    .def_static("baseTypeMax", &T::baseTypeMax,"baseTypeMax() max value of the base type of the vector")
    .def_static("baseTypeLowest", &T::baseTypeLowest,"baseTypeLowest() largest negative value of the base type of the vector")
    .def_static("baseTypeSmallest", &T::baseTypeSmallest,"baseTypeSmallest() smallest value of the base type of the vector")
    .def("length", &T::length, "length() magnitude of the vector")
    .def("length2", &T::length2,"length2() square magnitude of the vector")
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
    .def("negate", &T::negate)
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
    .def_readwrite("x", &T::x)
    .def_readwrite("y", &T::y)
    .def_readwrite("z", &T::z);

    register_vec<T>(c);
}

template <class T, class S>
void register_vec4(pybind11::module& m, const char * name)
{
    // no % or %=
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
    register_vec2<Imath::V2d, double>(m, "V2d");
    register_vec2<Imath::V2f, float>(m, "V2f");

    register_vec3<Imath::V3d, double>(m, "V3d");
    register_vec3<Imath::V3f, float>(m, "V3f");

    register_vec4<Imath::V4d, double>(m, "V4d");
    register_vec4<Imath::V4f, float>(m, "V4f");
}

}