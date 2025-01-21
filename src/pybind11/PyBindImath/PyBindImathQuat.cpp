//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"

#include <ImathVec.h>
#include <ImathMatrixAlgo.h>
#include <ImathEuler.h>

namespace PyBindImath {

namespace py = pybind11;

using IMATH_NAMESPACE::Quat;
using IMATH_NAMESPACE::Vec3;
using IMATH_NAMESPACE::Matrix33;
using IMATH_NAMESPACE::Matrix44;
using IMATH_NAMESPACE::Euler;

using IMATH_NAMESPACE::Quatf;
using IMATH_NAMESPACE::Quatd;

namespace {

template<typename T>
struct GetClassName {};

template <>
struct GetClassName<Quatf> {
    static constexpr char const* value = "Quatf";
};

template <>
struct GetClassName<Quatd> {
    static constexpr char const* value = "Quatd";
};

template <typename Q>
std::string Quat_str(Q const& v)
{
    std::stringstream stream;
    stream << GetClassName<Q>::value << '(' 
        << v[0] << ", " 
        << v[1] << ", " 
        << v[2] << ", " 
        << v[3] << ')';
    return stream.str();
}

template <typename Q, typename T = typename Q::value_type>
std::string Quat_repr(Q const& v)
{
    static_assert(std::is_floating_point<T>::value, "Quat_repr requires a floating-point type");

    // Set precision based on the type
    constexpr int precision = std::is_same<T, float>::value ? 9 : 17;

    std::ostringstream oss;
    oss << GetClassName<Q>::value << '('
        << std::setprecision(precision) << v[0] << ", "
        << std::setprecision(precision) << v[1] << ", "
        << std::setprecision(precision) << v[2] << ", "
        << std::setprecision(precision) << v[3] << ')';
    return oss.str();
}

} // namespace

template <class Q, typename T = typename Q::value_type, typename E = Euler<T>>
void register_Quat(py::module& m)
{
    auto className = GetClassName<Q>::value;

    py::class_<Q>(m, className)
    .def(py::init<>(), "imath Quat initialization")
    .def(py::init<Quat<float>>(), "imath Quat copy initialization")
    .def(py::init<Quat<double>>(), "imath Quat copy initialization")
    .def(py::init<T, T, T, T>(), "make Quat from components")
    .def(py::init<T, Vec3<T>>(), "make Quat from components")
    .def("__init__", [](Q& self, E const& euler){new (&self) Q(euler.toQuat());})
    .def("__init__", [](Q& self, Matrix33<T> const& mat){new (&self) Q(E(mat).toQuat());})
    .def("__init__", [](Q& self, Matrix44<T> const& mat){new (&self) Q(E(mat).toQuat());})
    .def("identity",&Q::identity, 
        "q.identity() -- return an identity quaternion\n")
    .def("invert", &Q::invert,
        "q.invert() -- inverts quaternion q\n"
        "(modifying q); returns q")
            
    .def("inverse", &Q::inverse,
        "q.inverse() -- returns the inverse of\n"
        "quaternion q; q is not modified\n")
            
    .def("normalize", &Q::normalize,
        "q.normalize() -- normalizes quaternion q\n"
        "(modifying q); returns q")
        
    .def("normalized", &Q::normalized,
        "q.normalized() -- returns a normalized version\n"
        "of quaternion q; q is not modified\n")
            
    .def("length", &Q::length)

    .def("rotateVector", &Q::rotateVector,
        "q.rotateVector(orig) -- Given a vector orig,\n"
        "   calculate orig' = q x orig x q*\n\n"
        "   Assumes unit quaternions")
            
    .def("setAxisAngle", &Q::setAxisAngle,
        "q.setAxisAngle(x,r) -- sets the value of\n"
        "quaternion q so that q represents a rotation\n"
        "of r radians around axis x")
            
    .def("setRotation", &Q::setRotation,
        "q.setRotation(v,w) -- sets the value of\n"
        "quaternion q so that rotating vector v by\n"
        "q produces vector w")
            
    .def("angle", &Q::angle,
        "q.angle() -- returns the rotation angle\n"
        "(in radians) represented by quaternion q")
            
    .def("axis", &Q::axis,
        "q.axis() -- returns the rotation axis\n"
        "represented by quaternion q")
            
    .def("toMatrix33", &Q::toMatrix33,
        "q.toMatrix33() -- returns a 3x3 matrix that\n"
        "represents the same rotation as quaternion q")
            
    .def("toMatrix44", &Q::toMatrix44,
        "q.toMatrix44() -- returns a 4x4 matrix that\n"
        "represents the same rotation as quaternion q")
            
    .def("log",&Q::log)
    .def("exp",&Q::exp)
    .def_readwrite("v",&Q::v)                       
    .def_readwrite("r",&Q::r)
    .def("v", [](Q const& self) { return self.v; },
        "q.v() -- returns the v (vector) component\n"
        "of quaternion q")
            
    .def("r", [](Q const& self) { return self.r; },
        "q.r() -- returns the r (scalar) component\n"
        "of quaternion q")
                    
    .def("setR", [](Q self, T r){ self.r = r; },
        "q.setR(s) -- sets the r (scalar) component\n"
        "of quaternion q to s")
            
    .def("setV", [](Q& self, Vec3<T> const& v){ self.v = v; },
        "q.setV(w) -- sets the v (vector) component\n"
        "of quaternion q to w")
            
    .def("extract", [](Q const& self, Matrix44<T> const& mat){ return IMATH_NAMESPACE::extractQuat(mat); },
        "q.extract(m) -- extracts the rotation component\n"
        "from 4x4 matrix m and stores the result in q")
            
    .def("slerp", [](Q const& self, Q const& other, T t){return IMATH_NAMESPACE::slerp (self, other, t);},
        "q.slerp(p,t) -- performs sperical linear\n"
        "interpolation between quaternions q and p:\n"
        "q.slerp(p,0) returns q; q.slerp(p,1) returns p.\n"
        "q and p must be normalized\n")

    .def("slerpShortestArc", [](Q const& self, Q const& other, T t){ return IMATH_NAMESPACE::slerpShortestArc(self, other, t); },
        "q.slerpShortestArc(p,t) -- performs spherical linear\n"
        "interpolation along the shortest arc between\n"
        "quaternions q and either p or -p, whichever is\n"
        "closer. q and p must be normalized\n")
            
    .def("__str__", Quat_str<Q>)
    .def("__repr__", Quat_repr<Q>)
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

    .def(py::self * Matrix33<T>())
    .def(Matrix33<T>() * py::self)

    .def(-py::self)
    .def("__invert__", [](Q const& self){return ~self;})
    .def("__xor__", [](Q const& self, Q const& other){return self ^ other;})
    ;
}

void register_imath_quat(py::module& m)
{
    register_Quat<Quatf>(m);
    register_Quat<Quatd>(m);
}
		 
} // PyBindImath
