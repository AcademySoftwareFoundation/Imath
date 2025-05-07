//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathBox.h>

namespace {

template <class T, class V>
void register_box(pybind11::class_<T>& c)
{
    c.def(pybind11::init<>())
    .def(pybind11::init<const V &>())
    .def(pybind11::init<const V &, const V &>())
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("makeEmpty", &T::makeEmpty, "makeEmpty() make the box empty")
    .def("makeInfinite", &T::makeInfinite, "makeInfinite() make the box cover all space")
    .def_readwrite("min", &T::min)
    .def_readwrite("max", &T::max)
    .def("extendBy", pybind11::overload_cast<const T &>(&T::extendBy), "extendBy(point) extend the box by a box")
    .def("extendBy", pybind11::overload_cast<const V &>(&T::extendBy), "extendBy(point) extend the box by a point")
    .def("size", &T::size, "size() size of the box")
    .def("center", &T::center, "center() center of the box")
    .def("intersects",pybind11::overload_cast<const V &>(&T::extendBy), "intersects(point) returns true if the box intersects the given point")
    .def("intersects",pybind11::overload_cast<const T &>(&T::extendBy), "intersects(box) returns true if the box intersects the given box")
    .def("majorAxis",&T::majorAxis, "majorAxis() major axis of the box")
    .def("isEmpty",&T::isEmpty, "isEmpty() returns true if the box is empty")
    .def("isInfinite",&T::isInfinite, "isInfinite() returns true if the box covers all space")
    .def("hasVolume",&T::hasVolume, "hasVolume() returns true if the box has volume")
        ;

}

template <class T, class V>
void register_box2(pybind11::module& m, const char * name)
{
    pybind11::class_<T> c(m, name);
    register_box<T, V>(c);
}

template <class T, class V>
void register_box3(pybind11::module& m, const char * name)
{
    pybind11::class_<T> c(m, name);
    register_box<T, V>(c);
}

} // namespace

namespace PyBindImath {
 
void register_imath_box(pybind11::module& m)
{
    register_box2<IMATH_NAMESPACE::Box2d, IMATH_NAMESPACE::V2d>(m, "Box2d");
    register_box2<IMATH_NAMESPACE::Box2f, IMATH_NAMESPACE::V2f>(m, "Box2f");

    register_box3<IMATH_NAMESPACE::Box3d, IMATH_NAMESPACE::V3d>(m, "Box3d");
    register_box3<IMATH_NAMESPACE::Box3f, IMATH_NAMESPACE::V3f>(m, "Box3f");
}

} // namespace PyBindImath

