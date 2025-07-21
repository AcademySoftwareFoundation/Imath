//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"
#include <ImathLine.h>
#include <ImathLineAlgo.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Line>
void
register_line(py::module& m, const char *name)
{
    typedef typename Line::BaseType T;
    typedef Vec3<T> Vec;
    
    py::class_<Line> c(m, name);    
    c.attr("__module__") = "";
    c.def(py::init<>([](){
        Vec pos(T(0),T(0),T(0));
        Vec dir(T(1),T(0),T(0));
        Line line(pos, dir);
        return line;
    } ))
        .def(py::init<const Vec3<float>&, const Vec3<float>&>(), py::arg("point1"), py::arg("point2"), "Initialize with two points. The direction is the difference between the points.")
        .def(py::init<const Vec3<double>&, const Vec3<double>&>(), py::arg("point1"), py::arg("point2"), "Initialize with two points. The direction is the difference between the points.")
        .def(py::init([](const Line3<float>& other) {
            Line l;
            l.pos = Vec(other.pos);
            l.dir = Vec(other.dir);
            return l;
        }))
        .def(py::init([](const Line3<double>& other) {
            Line l;
            l.pos = Vec(other.pos);
            l.dir = Vec(other.dir);
            return l;
        }))
        .def("__repr__", [name](const Line &self) {
            std::stringstream ss;
            Vec p2 = self.pos + self.dir;
            ss << name << "(" << py::repr(py::cast(self.pos)) << ", " << py::repr(py::cast(p2)) << ")";
            return ss.str();
        })
        .def("__eq__", [](const Line& self, const Line& other) { return self.pos == other.pos && self.dir == other.dir; })
        .def("__ne__", [](const Line& self, const Line& other) { return self.pos != other.pos || self.dir != other.dir; })
#if XXX
        .def_readwrite("pos", &Line::pos)
        .def_readwrite("dir", &Line::dir)
#endif
        .def("pos", [](const Line& self) { return self.pos; })
        .def("dir", [](const Line& self) { return self.dir; })
        .def("setPos", [](Line& self, const Vec& v) { self.pos = v; })
        .def("setDir", [](Line& self, const Vec& v) { self.dir = v.normalized(); })
        .def("set", [](Line& self, const Vec& pos, const Vec& dir) { self.set(pos, dir); })
        .def(py::self * Matrix44<T>())

        .def("pointAt", [](Line& self, T t) { return self.operator()(t); },
             "l.pointAt(t) -- returns l.pos() + t * l.dir()")
        
        .def("distanceTo", [](Line& self, Vec& p) { return self.distanceTo(p); },
             "l.distanceTo(p) -- returns the distance from\n"
             "   line l to point p\n")
                                        
        .def("distanceTo", [](Line& self, Line& l) { return self.distanceTo(l); },
             "l1.distanceTo(l2) -- returns the distance from\n"
             "   line l1 to line l2\n")
        
        .def("distanceTo", [](Line& self, const py::tuple& t) { return self.distanceTo(vecFromTuple<Vec>(t)); })
        .def("closestPointTo", [](Line& self, Vec& p) { return self.closestPointTo(p); },
             "l.closestPointTo(p) -- returns the point on\n"
             "   line l that is closest to point p\n")
        .def("closestPointTo", [](Line& self, Line& l) { return self.closestPointTo(l); },
             "l.closestPointTo(p) -- returns the point on\n"
             "   line l that is closest to line l\n")
        .def("closestPointTo", [](Line& self, const py::tuple& t) { return self.closestPointTo(vecFromTuple<Vec>(t)); })
        
        .def("closestPoints", [](const Line& self, const Line& other, Vec& p0, Vec& p1) { closestPoints(self, other, p0, p1); },
             "l1.closestPoints(l2,p0,p1)")    
        .def("closestPoints", [](const Line& self, const Line& other) {
            Vec p0, p1;
            closestPoints(self, other, p0, p1);
            return py::make_tuple(p0, p1);},
            "l1.closestPoints(l2,p0,p1)")    
        
        .def("closestTriangleVertex", [](const Line& self, const Vec& v0, const Vec& v1, const Vec& v2) {
            return closestVertex(v0, v1, v2, self);},
             "l.closestTriangleVertex(v0, v1, v2) -- returns\n"
             "a copy of v0, v1, or v2, depending on which is\n"
             "closest to line l.\n")
        .def("closestTriangleVertex", [](const Line& self, const py::tuple& t0, const py::tuple& t1, const py::tuple& t2) {
            Vec v0 = vecFromTuple<Vec>(t0);
            Vec v1 = vecFromTuple<Vec>(t1);
            Vec v2 = vecFromTuple<Vec>(t2);
            return closestVertex(v0, v1, v2, self);
        })
        .def("intersectWithTriangle", [](const Line& self, const Vec& v0, const Vec& v1, const Vec& v2) -> py::object {
            Vec pt, bar;
            bool front;
            if (intersect(self, v0, v1, v2, pt, bar, front))
                return py::make_tuple(pt, bar, front);
            return py::none();
        })
        .def("intersectWithTriangle", [](const Line& self, const py::tuple& t0, const py::tuple& t1, const py::tuple& t2) {
            Vec v0 = vecFromTuple<Vec>(t0);
            Vec v1 = vecFromTuple<Vec>(t1);
            Vec v2 = vecFromTuple<Vec>(t2);
            return closestVertex(v0, v1, v2, self);
        })
        .def("intersectWithTriangle", [](const Line& self, const Vec& v0, const Vec& v1, const Vec& v2, Vec& pt, Vec& barycentric, bool& front) {
            return intersect(self, v0, v1, v2, pt, barycentric, front);},
            "l.intersectWithTriangle(v0, v1, v2) -- computes the\n"
            "intersection of line l and triangle (v0, v1, v2).\n"
            "\n"
            "If the line and the triangle do not intersect,\n"
            "None is returned.\n"
            ""
            "If the line and the triangle intersect, a tuple\n"
            "(p, b, f) is returned:\n"
            "\n"
            "   p  intersection point in 3D space\n"
            "\n"
            "   b  intersection point in barycentric coordinates\n"
            "\n"
            "   f  1 if the line hits the triangle from the\n"
            "      front (((v2-v1) % (v1-v2)) ^ l.dir() < 0),\n"
            "      0 if the line hits the trianble from the\n"
            "      back\n"
            "\n")
            
        .def("rotatePoint", [](const Line& self, const Vec& p, T r) {
            return rotatePoint(p, self, r); },
            "l.rotatePoint(p,r) -- rotates point p around\n"
            "line by angle r (in radians), and returns the\n"
            "result (p is not modified)\n")
        ;
}

} // namespace
 
namespace PyBindImath {
 
void
register_imath_line(py::module &m) 
{
    register_line<Line3f>(m, "Line3f");
    register_line<Line3d>(m, "Line3d");
}

} // namespace PyBindImath

