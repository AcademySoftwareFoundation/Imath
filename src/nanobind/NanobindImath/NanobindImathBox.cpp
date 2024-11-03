//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "NanobindImath.h"
#include <ImathBox.h>

namespace NanobindImath {

template <class T, class V>
void register_box(nanobind::class_<T>& c)
{
    c.def(nanobind::init<>())
    .def(nanobind::init<const V &>())
    .def(nanobind::init<const V &, const V &>())
    .def(nanobind::self == nanobind::self)
    .def(nanobind::self != nanobind::self)
    .def("makeEmpty", &T::makeEmpty, "makeEmpty() make the box empty")
    .def("makeInfinite", &T::makeInfinite, "makeInfinite() make the box cover all space")
    .def_rw("min", &T::min)
    .def_rw("max", &T::max)
    .def("extendBy", nanobind::overload_cast<const T &>(&T::extendBy), "extendBy(point) extend the box by a box")
    .def("extendBy", nanobind::overload_cast<const V &>(&T::extendBy), "extendBy(point) extend the box by a point")
    .def("size", &T::size, "size() size of the box")
    .def("center", &T::center, "center() center of the box")
    .def("intersects",nanobind::overload_cast<const V &>(&T::extendBy), "intersects(point) returns true if the box intersects the given point")
    .def("intersects",nanobind::overload_cast<const T &>(&T::extendBy), "intersects(box) returns true if the box intersects the given box")
    .def("majorAxis",&T::majorAxis, "majorAxis() major axis of the box")
    .def("isEmpty",&T::isEmpty, "isEmpty() returns true if the box is empty")
    .def("isInfinite",&T::isInfinite, "isInfinite() returns true if the box covers all space")
    .def("hasVolume",&T::hasVolume, "hasVolume() returns true if the box has volume");

}

template <class T, class V>
void register_box2(nanobind::module_& m, const char * name)
{
    nanobind::class_<T> c(m, name);
    register_box<T, V>(c);
}

template <class T, class V>
void register_box3(nanobind::module_& m, const char * name)
{
    nanobind::class_<T> c(m, name);
    register_box<T, V>(c);
}

void register_imath_box(nanobind::module_& m)
{
    register_box2<Imath::Box2d, Imath::V2d>(m, "Box2d");
    register_box2<Imath::Box2f, Imath::V2f>(m, "Box2f");

    register_box3<Imath::Box3d, Imath::V3d>(m, "Box3d");
    register_box3<Imath::Box3f, Imath::V3f>(m, "Box3f");

}

}
