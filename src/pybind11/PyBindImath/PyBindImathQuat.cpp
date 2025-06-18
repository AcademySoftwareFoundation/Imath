//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"

#include <ImathVec.h>
#include <ImathMatrixAlgo.h>
#include <ImathEuler.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Quat>
void
register_quat(py::module& m, const char* name)
{
    typedef typename Quat::value_type T;
    
    py::class_<Quat> quat(m, name);
    quat.attr("__module__") = "";
    quat.def(py::init<>(), "imath Quat initialization")
        .def(py::init<Quatf>(), "imath Quat copy initialization")
        .def(py::init<Quatd>(), "imath Quat copy initialization")
        .def(py::init<T, Vec3<T>>(), "make Quat from components")
        .def(py::init([](T s, T i, T j, T k) { return Quat(s, i, j, k); }))
        .def(py::init([](const Euler<T>& euler) { return euler.toQuat();}))
        .def(py::init([](const Matrix33<T>& mat) { Euler<T> e(mat); return e.toQuat();}))
        .def(py::init([](const Matrix44<T>& mat) { Euler<T> e(mat); return e.toQuat();}))
        .def("identity", [](const Quat& self) { return Quat(); },
             "q.identity() -- return an identity quaternion\n")
        .def("invert", &Quat::invert,
             "q.invert() -- inverts quaternion q\n"
             "(modifying q); returns q")
            
        .def("inverse", &Quat::inverse,
             "q.inverse() -- returns the inverse of\n"
             "quaternion q; q is not modified\n")
            
        .def("normalize", &Quat::normalize,
             "q.normalize() -- normalizes quaternion q\n"
             "(modifying q); returns q")
        
        .def("normalized", &Quat::normalized,
             "q.normalized() -- returns a normalized version\n"
             "of quaternion q; q is not modified\n")
            
        .def("length", &Quat::length)

        .def("rotateVector", &Quat::rotateVector,
             "q.rotateVector(orig) -- Given a vector orig,\n"
             "   calculate orig' = q x orig x q*\n\n"
             "   Assumes unit quaternions")
            
        .def("setAxisAngle", &Quat::setAxisAngle,
             "q.setAxisAngle(x,r) -- sets the value of\n"
             "quaternion q so that q represents a rotation\n"
             "of r radians around axis x")
            
        .def("setRotation", &Quat::setRotation,
             "q.setRotation(v,w) -- sets the value of\n"
             "quaternion q so that rotating vector v by\n"
             "q produces vector w")
            
        .def("angle", &Quat::angle,
             "q.angle() -- returns the rotation angle\n"
             "(in radians) represented by quaternion q")
            
        .def("axis", &Quat::axis,
             "q.axis() -- returns the rotation axis\n"
             "represented by quaternion q")
            
        .def("toMatrix33", &Quat::toMatrix33,
             "q.toMatrix33() -- returns a 3x3 matrix that\n"
             "represents the same rotation as quaternion q")
            
        .def("toMatrix44", &Quat::toMatrix44,
             "q.toMatrix44() -- returns a 4x4 matrix that\n"
             "represents the same rotation as quaternion q")
            
        .def("log",&Quat::log)
        .def("exp",&Quat::exp)
#if XXX
        .def_readwrite("v",&Quat::v)                       
        .def_readwrite("r",&Quat::r)
#endif
        .def("v", [](const Quat& self) { return self.v; },
             "q.v() -- returns the v (vector) component\n"
             "of quaternion q")
        .def("r", [](const Quat& self) { return self.r; },
             "q.r() -- returns the r (scalar) component\n"
             "of quaternion q")
        
        .def("setR", [](Quat self, T r){ self.r = r; },
             "q.setR(s) -- sets the r (scalar) component\n"
             "of quaternion q to s")
            
        .def("setV", [](Quat& self, Vec3<T> const& v){ self.v = v; },
             "q.setV(w) -- sets the v (vector) component\n"
             "of quaternion q to w")
            
        .def("extract", [](Quat& self, const Matrix44<T>& mat) { self = extractQuat(mat); return self; },
             "q.extract(m) -- extracts the rotation component\n"
             "from 4x4 matrix m and stores the result in q")
            
        .def("slerp", [](const Quat& self, const Quat& other, T t){return IMATH_NAMESPACE::slerp (self, other, t);},
             "q.slerp(p,t) -- performs sperical linear\n"
             "interpolation between quaternions q and p:\n"
             "q.slerp(p,0) returns q; q.slerp(p,1) returns p.\n"
             "q and p must be normalized\n")

        .def("slerpShortestArc", [](const Quat& self, const Quat& other, T t){ return IMATH_NAMESPACE::slerpShortestArc(self, other, t); },
             "q.slerpShortestArc(p,t) -- performs spherical linear\n"
             "interpolation along the shortest arc between\n"
             "quaternions q and either p or -p, whichever is\n"
             "closer. q and p must be normalized\n")
            
        .def(py::self == py::self)
        .def(py::self != py::self)
        
        .def(T() * py::self)

        .def(py::self * T())
        .def(py::self / T())

        .def(py::self *= T())
        .def(py::self /= T())

        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
    
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)

        .def("__mul__", [](const Quat& self, const Matrix33<T>& mat) { return self * mat; })
        .def("__rmul__", [](const Quat& self, const Matrix33<T>& mat) { return mat * self; })

        .def("__rmul__", [](const Quat& self, const Vec3<T>& v) { return v * self; })

        .def(-py::self)

        .def("__invert__", [](const Quat& self){return ~self;})
        .def("__xor__", [](const Quat& self, const Quat& other){return self ^ other;})

        .def("__repr__", [name](const Quat& q) {
            std::stringstream stream;
            if (std::is_same<T, float>::value) {
                stream.precision(9);
            } else if (std::is_same<T, double>::value) {
                stream.precision(17);
            }
            stream << name << "("
                   << q.r << ", " 
                   << q.v[0] << ", " 
                   << q.v[1] << ", " 
                   << q.v[2] << ')';
            return stream.str();
        })
        ;
}

} // namespace

namespace PyBindImath {

void
register_imath_quat(py::module& m)
{
    register_quat<Quatf>(m, "Quatf");
    register_quat<Quatd>(m, "Quatd");
}
		 
} // PyBindImath
