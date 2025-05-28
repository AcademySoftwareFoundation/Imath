//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"
#include <ImathBox.h>
#include <ImathBoxAlgo.h>
#include <ImathMatrix.h>

namespace py = pybind11;

namespace {

template <class Box, class Vec>
py::class_<Box>
register_box(const char* name, py::class_<Box>& c)
{
    c
        .def(py::init<>())
        .def(py::init<const Vec &>())
        .def(py::init<const Vec &, const Vec &>())
        .def(py::init([](py::tuple t)
            {
                if (t.size() != 2)
                    throw std::runtime_error("Box constructor requires two arguments");
                return Box(vecFromObject<Vec>(t[0]), vecFromObject<Vec>(t[1]));
            }))
        .def("__repr__", [name](const Box& b) {
            py::object min_obj = py::cast(b.min);
            py::object max_obj = py::cast(b.max);
            return py::str("{}({}, {})").format(name, py::repr(min_obj), py::repr(max_obj));
        })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("makeEmpty", &Box::makeEmpty, "makeEmpty() make the box empty")
        .def("makeInfinite", &Box::makeInfinite, "makeInfinite() make the box cover all space")
        .def("min", [](const Box& self) { return self.min; }, "min() returns the minimum point of the box")
        .def("max", [](const Box& self) { return self.max; }, "max() returns the maximum point of the box")
        .def("setMin", [](Box& self, const Vec& v) { self.min = v; }, "setMin(point) sets the minimum point of the box")
        .def("setMax", [](Box& self, const Vec& v) { self.max = v; }, "setMax(point) sets the maximum point of the box")
        .def("size", &Box::size, "size() size of the box")
        .def("center", &Box::center, "center() center of the box")
        .def("extendBy", py::overload_cast<const Box &>(&Box::extendBy), "extendBy(box) extend the box by a box")
        .def("extendBy", py::overload_cast<const Vec &>(&Box::extendBy), "extendBy(point) extend the box by a point")
        .def("intersects", py::overload_cast<const Vec &>(&Box::intersects, py::const_), "intersects(point) returns true if the box intersects the given point")
        .def("intersects", py::overload_cast<const Box &>(&Box::intersects, py::const_), "intersects(box) returns true if the box intersects the given box")
        .def("majorAxis", &Box::majorAxis, "majorAxis() major axis of the box")
        .def("isEmpty", &Box::isEmpty, "isEmpty() returns true if the box is empty")
        .def("isInfinite", &Box::isInfinite, "isInfinite() returns true if the box covers all space")
        .def("hasVolume", &Box::hasVolume, "hasVolume() returns true if the box has volume");

    return c;
}


template <class Box, class Matrix>
py::class_<Box>
register_box_mul(py::class_<Box>& c)
{ 
    auto ri = py::return_value_policy::reference_internal;

    c.def("__mul__", [](const Box& self, const Matrix& M) { return IMATH_NAMESPACE::transform(self, M); });
    c.def("__imul__", [](Box& self, const Matrix& M) -> const Box& { self = IMATH_NAMESPACE::transform(self, M); return self; }, ri);
    return c;
}

template <class Box, class Vec>
py::class_<Box>
register_box2(py::module& m, const char * name)
{
    py::class_<Box> c(m, name);
    c.attr("__module__") = "";
    c.def(py::init([](const IMATH_NAMESPACE::Box<Vec2<float>>& other) {
        return Box(Vec(other.min), Vec(other.max));
    }))
        .def(py::init([](const IMATH_NAMESPACE::Box<Vec2<double>>& other) {
            return Box(Vec(other.min), Vec(other.max));
        }))
        .def(py::init([](const IMATH_NAMESPACE::Box<Vec2<int>>& other) {
            return Box(Vec(other.min), Vec(other.max));
        }))
        .def(py::init([](const IMATH_NAMESPACE::Box<Vec2<int64_t>>& other) {
            return Box(Vec(other.min), Vec(other.max));
        }))
        ;
    
    return register_box<Box, Vec>(name, c);
}

template <class Box, class Vec>
py::class_<Box>
register_box3(py::module& m, const char * name)
{
    py::class_<Box> c(m, name);
    c.def(py::init([](const IMATH_NAMESPACE::Box<Vec3<float>>& other) {
        return Box(Vec(other.min), Vec(other.max));
    }))
        .def(py::init([](const IMATH_NAMESPACE::Box<Vec3<double>>& other) {
            return Box(Vec(other.min), Vec(other.max));
        }))
        .def(py::init([](const IMATH_NAMESPACE::Box<Vec3<int>>& other) {
            return Box(Vec(other.min), Vec(other.max));
        }))
        .def(py::init([](const IMATH_NAMESPACE::Box<Vec3<int64_t>>& other) {
            return Box(Vec(other.min), Vec(other.max));
        }))
        ;
    return register_box<Box, Vec>(name, c);
}

} // namespace

namespace PyBindImath {
 
void register_imath_box(py::module& m)
{
    // Register 2D boxes
    register_box2<IMATH_NAMESPACE::Box2i, IMATH_NAMESPACE::V2i>(m, "Box2i");
    register_box2<IMATH_NAMESPACE::Box2i64, IMATH_NAMESPACE::V2i64>(m, "Box2i64");
    register_box2<IMATH_NAMESPACE::Box2f, IMATH_NAMESPACE::V2f>(m, "Box2f");
    register_box2<IMATH_NAMESPACE::Box2d, IMATH_NAMESPACE::V2d>(m, "Box2d");

    // Register 3D boxes
    auto box3i = register_box3<IMATH_NAMESPACE::Box3i, IMATH_NAMESPACE::V3i>(m, "Box3i");
    register_box_mul<IMATH_NAMESPACE::Box3i,IMATH_NAMESPACE::M44f>(box3i);
    register_box_mul<IMATH_NAMESPACE::Box3i,IMATH_NAMESPACE::M44d>(box3i);
  
    auto box3i64 = register_box3<IMATH_NAMESPACE::Box3i64, IMATH_NAMESPACE::V3i64>(m, "Box3i64");
    register_box_mul<IMATH_NAMESPACE::Box3i64,IMATH_NAMESPACE::M44f>(box3i64);
    register_box_mul<IMATH_NAMESPACE::Box3i64,IMATH_NAMESPACE::M44d>(box3i64);

    auto box3f = register_box3<IMATH_NAMESPACE::Box3f, IMATH_NAMESPACE::V3f>(m, "Box3f");
    register_box_mul<IMATH_NAMESPACE::Box3f,IMATH_NAMESPACE::M44f>(box3f);
    register_box_mul<IMATH_NAMESPACE::Box3f,IMATH_NAMESPACE::M44d>(box3f);
    
    auto box3d = register_box3<IMATH_NAMESPACE::Box3d, IMATH_NAMESPACE::V3d>(m, "Box3d");
    register_box_mul<IMATH_NAMESPACE::Box3d,IMATH_NAMESPACE::M44f>(box3d);
    register_box_mul<IMATH_NAMESPACE::Box3d,IMATH_NAMESPACE::M44d>(box3d);
}

} // namespace PyBindImath

