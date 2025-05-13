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
    .def(pybind11::init<const T &>())  // Copy constructor
    .def(pybind11::init<const std::pair<V, V> &>())  // Constructor from pair of points
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("makeEmpty", &T::makeEmpty, "makeEmpty() make the box empty")
    .def("makeInfinite", &T::makeInfinite, "makeInfinite() make the box cover all space")
    .def("min", &T::min, "min() returns the minimum point of the box")
    .def("max", &T::max, "max() returns the maximum point of the box")
    .def("setMin", &T::setMin, "setMin(point) sets the minimum point of the box")
    .def("setMax", &T::setMax, "setMax(point) sets the maximum point of the box")
    .def("extendBy", pybind11::overload_cast<const T &>(&T::extendBy), "extendBy(box) extend the box by a box")
    .def("extendBy", pybind11::overload_cast<const V &>(&T::extendBy), "extendBy(point) extend the box by a point")
    .def("size", &T::size, "size() size of the box")
    .def("center", &T::center, "center() center of the box")
    .def("intersects", pybind11::overload_cast<const V &>(&T::intersects), "intersects(point) returns true if the box intersects the given point")
    .def("intersects", pybind11::overload_cast<const T &>(&T::intersects), "intersects(box) returns true if the box intersects the given box")
    .def("majorAxis", &T::majorAxis, "majorAxis() major axis of the box")
    .def("isEmpty", &T::isEmpty, "isEmpty() returns true if the box is empty")
    .def("isInfinite", &T::isInfinite, "isInfinite() returns true if the box covers all space")
    .def("hasVolume", &T::hasVolume, "hasVolume() returns true if the box has volume");

    // Add transform operations for 3D boxes
    if constexpr (std::is_same_v<T, IMATH_NAMESPACE::Box3f> || std::is_same_v<T, IMATH_NAMESPACE::Box3d>)
    {
        c.def(pybind11::self * IMATH_NAMESPACE::M44f())
         .def(pybind11::self * IMATH_NAMESPACE::M44d())
         .def(pybind11::self *= IMATH_NAMESPACE::M44f())
         .def(pybind11::self *= IMATH_NAMESPACE::M44d());
    }
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
    // Register 2D boxes
    register_box2<IMATH_NAMESPACE::Box2i, IMATH_NAMESPACE::V2i>(m, "Box2i");
    register_box2<IMATH_NAMESPACE::Box2i64, IMATH_NAMESPACE::V2i64>(m, "Box2i64");
    register_box2<IMATH_NAMESPACE::Box2f, IMATH_NAMESPACE::V2f>(m, "Box2f");
    register_box2<IMATH_NAMESPACE::Box2d, IMATH_NAMESPACE::V2d>(m, "Box2d");

    // Register 3D boxes
    register_box3<IMATH_NAMESPACE::Box3i, IMATH_NAMESPACE::V3i>(m, "Box3i");
    register_box3<IMATH_NAMESPACE::Box3i64, IMATH_NAMESPACE::V3i64>(m, "Box3i64");
    register_box3<IMATH_NAMESPACE::Box3f, IMATH_NAMESPACE::V3f>(m, "Box3f");
    register_box3<IMATH_NAMESPACE::Box3d, IMATH_NAMESPACE::V3d>(m, "Box3d");
}

} // namespace PyBindImath

