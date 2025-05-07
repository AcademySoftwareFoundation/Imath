//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathPlane.h>
#include <ImathVec.h>
#include <ImathLine.h>

namespace {

template <class T, class L, class Q, class S>
void register_plane(pybind11::module& m, const char *name)
{
    pybind11::class_<T> c(m, name);
    c.def(pybind11::init<>(), "Uninitialized by default")
    .def(pybind11::init<const Q &, S>(), pybind11::arg("normal"), pybind11::arg("distance"), "Initialize with a normal and distance")
    .def(pybind11::init<const Q &, const Q &>(), pybind11::arg("point"), pybind11::arg("normal"), "Initialize with a point and a normal")
    .def(pybind11::init<const Q &, const Q &, const Q &>(), pybind11::arg("point1"), pybind11::arg("point2"), pybind11::arg("point3"), "Initialize with three points")
    .def_readwrite("normal", &T::normal, "The normal to the plane")
    .def_readwrite("distance", &T::distance, "The distance from the origin to the plane")
    .def("set", pybind11::overload_cast<const Q &, S>(&T::set), pybind11::arg("normal"), pybind11::arg("distance"), "Set via a given normal and distance")
    .def("set", pybind11::overload_cast<const Q &, const Q &>(&T::set), pybind11::arg("point"), pybind11::arg("normal"), "Set via a given point and normal")
    .def("set", pybind11::overload_cast<const Q &, const Q &, const Q &>(&T::set), pybind11::arg("point1"), pybind11::arg("point2"), pybind11::arg("point3"), "Set via three points")
    .def("intersect", [](T& self, const L& line) {
                Q intersection;
                bool result = self.intersect(line, intersection);
                return pybind11::make_tuple(result, intersection);
            }, pybind11::arg("line"), "Determine if a line intersects the plane. Returns a tuple (bool, Vec3). True if the line intersects the plane. Second element is the point of intersection")
    .def("intersectT", [](T& self, const L& line) {
                S intersection;
                bool result = self.intersectT(line, intersection);
                return pybind11::make_tuple(result, intersection);
            }, pybind11::arg("line"), "Determine if a line intersects the plane. Returns a tuple (bool, T). True if the line intersects the plane. Second element is the parametric value of the point of intersection")
    .def("distanceTo", &T::distanceTo, pybind11::arg("point"), "Returns the distance from a point to the plane")
    .def("reflectPoint", &T::reflectPoint, pybind11::arg("point"), "Reflects the given point around the plane")
    .def("reflectVector", &T::reflectVector, pybind11::arg("v"), "Reflects the direction vector around the plane")
    .def("__str__", [](const T &obj) {
            std::stringstream ss;
            ss << obj;
            return ss.str();
        })
        ;
}

} // namespace
 
namespace PyBindImath {
    
void
register_imath_plane(pybind11::module &m) 
{
    register_plane<IMATH_NAMESPACE::Plane3f, IMATH_NAMESPACE::Line3f, IMATH_NAMESPACE::V3f, float>(m, "Plane3f");
    register_plane<IMATH_NAMESPACE::Plane3d, IMATH_NAMESPACE::Line3d, IMATH_NAMESPACE::V3d, double>(m, "Plane3d");
}

} // namespace PyBindImath  

