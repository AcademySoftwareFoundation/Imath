//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathPlane.h>
#include <ImathVec.h>
#include <ImathLine.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class T, class S>
static py::object
intersect1(const Plane3<T> &plane, const Line3<S> &line)
{
    Vec3<T> intersection;
    Line3<T> l;
    l.pos = line.pos;
    l.dir = line.dir;
    if(plane.intersect(l, intersection))
        return py::cast(intersection);
    return py::none();
}

template <class T, class S>
static py::object
intersectT(const Plane3<T> &plane, const Line3<S> &line)
{
    T param;
    Line3<T> l;
    l.pos = line.pos;
    l.dir = line.dir;
    if(plane.intersectT(l, param))
        return py::cast(param);
    return py::none();
}

template <class Plane>
void
register_plane(py::module& m, const char *name)
{
    typedef typename Plane::BaseType T;
    typedef Vec3<T> Vec;
    typedef Line3<T> Line;
    
    py::class_<Plane> c(m, name);
    c.attr("__module__") = "";
    c.def(py::init<>([](){
        Vec normal(T(1),T(0),T(0));
        Plane plane(normal, T(0));
        return plane;
    } ))
        .def(py::init<const Vec &, T>(), py::arg("normal"), py::arg("distance"), "Initialize with a normal and distance")
        .def(py::init<const Vec &, const Vec &>(), py::arg("point"), py::arg("normal"), "Initialize with a point and a normal")
        .def(py::init<const Vec &, const Vec &, const Vec &>(), py::arg("point1"), py::arg("point2"), py::arg("point3"), "Initialize with three points")
        .def(py::init([](const Plane3<float>& other) {
            Plane p;
            p.normal = Vec(other.normal);
            p.distance = T(other.distance);
            return p;
        }))
        .def(py::init([](const Plane3<double>& other) {
            Plane p;
            p.normal = Vec(other.normal);
            p.distance = T(other.distance);
            return p;
        }))
        .def("__repr__", [name](const Plane &self) {
            std::stringstream ss;
            if (std::is_same<T, float>::value) {
                ss.precision(9);
            } else if (std::is_same<T, double>::value) {
                ss.precision(17);
            }
            ss << name << "(" << py::repr(py::cast(self.normal)) << ", " << self.distance << ")";
            return ss.str();
        })
        .def("__eq__", [](const Plane& self, const Plane& other) {
            return self.normal == other.normal && self.distance == other.distance;
        })
        .def("__ne__", [](const Plane& self, const Plane& other) { return self.normal != other.normal || self.distance != other.distance; })
        .def("normal", [](const Plane& self) {return self.normal; })
        .def("distance", [](const Plane& self) {return self.distance; })
        .def("setNormal", [](Plane& self, const Vec& n) {return self.normal = n.normalized(); })
        .def("setDistance", [](Plane& self, T d) {return self.distance = d; })
#if XXX
        .def_readwrite("normal", &Plane::normal, "The normal to the plane")
        .def_readwrite("distance", &Plane::distance, "The distance from the origin to the plane")
#endif
        .def("set", py::overload_cast<const Vec&, T>(&Plane::set), py::arg("normal"), py::arg("distance"), "Set via a given normal and distance")
        .def("set", py::overload_cast<const Vec&, const Vec&>(&Plane::set), py::arg("point"), py::arg("normal"), "Set via a given point and normal")
        .def("set", py::overload_cast<const Vec&, const Vec&, const Vec&>(&Plane::set), py::arg("point1"), py::arg("point2"), py::arg("point3"), "Set via three points")
        .def("__neg__", [](Plane& self) { Plane p; p.set(-self.normal, -self.distance); return p; })
        .def(py::self * Matrix44<T>())
        .def("intersect", [](const Plane& self, const Line& line, Vec& intersection) {
            return self.intersect(line, intersection);
        })
        .def("intersect", intersect1<T, float>, "Determine if a line intersects the plane. Returns a tuple (bool, Vec3). True if the line intersects the plane. Second element is the point of intersection")
        .def("intersect", intersect1<T, double>, "Determine if a line intersects the plane. Returns a tuple (bool, Vec3). True if the line intersects the plane. Second element is the point of intersection")
        .def("intersectT", intersectT<T, float>, "Determine if a line intersects the plane. Returns a tuple (bool, T). True if the line intersects the plane. Second element is the parametric value of the point of intersection")
        .def("intersectT", intersectT<T, double>,  "Determine if a line intersects the plane. Returns a tuple (bool, T). True if the line intersects the plane. Second element is the parametric value of the point of intersection")
        .def("distanceTo", &Plane::distanceTo,  "Returns the distance from a point to the plane")
        .def("reflectPoint", &Plane::reflectPoint, "Reflects the given point around the plane")
        .def("reflectVector", &Plane::reflectVector, "Reflects the direction vector around the plane")
        ;
}

} // namespace
 
namespace PyBindImath {
    
void
register_imath_plane(py::module &m) 
{
    register_plane<Plane3f>(m, "Plane3f");
    register_plane<Plane3d>(m, "Plane3d");
}

} // namespace PyBindImath  

