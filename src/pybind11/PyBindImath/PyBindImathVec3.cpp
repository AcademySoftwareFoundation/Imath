//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"
#include <ImathVec.h>
#include <ImathVecAlgo.h>
#include <ImathColor.h>
#include <ImathColorAlgo.h>
#include <ImathMatrix.h>

//
// Wrappings for V3s, V3i, V3f, V3d
//
// The wrappings support inter-type operability, i.e:
//
//   v = V3i(1,2,3) + V3f(3,4,6)
//
// The wrappings also support constructors and operations between
// vectors and tuples and lists, i.e:
//
//   v = V3i((1,2,3))
//   v = V3i([1,2,3])
//   v = V3i(1,2,3) + (3,4,5)
//   v = V3i(1,2,3) + [3,4,5]
//

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Vec>
py::class_<Vec>
register_vec_fp3(py::class_<Vec> c)
{
    register_vec_mat<Vec,M33f>(c);
    register_vec_mat<Vec,M33d>(c);
    register_vec_mat<Vec,M44f>(c);
    register_vec_mat<Vec,M44d>(c);
    return register_vec_fp(c);
}

//
// Vec3<T>
//

template <class Vec>
py::class_<Vec>
register_vec3(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.attr("__module__") = "";
    c.def("__repr__", [name](const Vec& v) { return repr(name, v); })
        .def(py::init([](){return Vec(0);}))
        .def(py::init<short>())
        .def(py::init<short,short,short>())
        .def(py::init<int>())
        .def(py::init<int,int,int>())
        .def(py::init<float>())
        .def(py::init<float,float,float>())
        .def(py::init<double>())
        .def(py::init<double,double,double>())
        .def(py::init([](const py::tuple& o) { Vec v; return vecFromTuple<Vec>(o); }))
        .def(py::init([](const py::list& o) { Vec v; return vecFromList<Vec>(o); }))
        .def(py::self % py::self)
        .def(py::self %= py::self)
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def_readwrite("z", &Vec::z)
        .def("setValue", [](Vec& self, T x, T y, T z) { self.setValue(x, y, z); }, "set to the given x,y,z values")
        .def("cross", &Vec::cross, "return the right-handed cross product with the given vector")
        ;

    register_vec_geom<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

template <class Color>
py::class_<Color>
register_color3(py::module& m, const char * name)
{
    auto c = register_vec3<Color>(m, name);
    c.def("hsv2rgb", [](const Color& v) { return static_cast<Color>(hsv2rgb(v)); })
        .def("rgb2hsv", [](const Color& v) { return static_cast<Color>(rgb2hsv(v)); })
        .def_readwrite("r", &Color::x)
        .def_readwrite("g", &Color::y)
        .def_readwrite("b", &Color::z)
        ;
    return c;
}

} // namespace

namespace PyBindImath {

void
register_imath_color3(py::module& m)
{
    auto c3c = register_color3<C3c>(m, "Color3c");
    auto c3f = register_color3<C3f>(m, "Color3f");

    register_vec_arithmetic<C3c,C3c>(c3c);
    register_vec_arithmetic<C3c,C3f>(c3c);
    register_vec_arithmetic<C3c,V3i>(c3c);
    register_vec_arithmetic<C3c,V3f>(c3c);
    register_vec_arithmetic<C3c,V3d>(c3c);

    register_vec_arithmetic<C3f,C3f>(c3f);
    register_vec_arithmetic<C3f,C3c>(c3f);
    register_vec_arithmetic<C3f,V3i>(c3f);
    register_vec_arithmetic<C3f,V3f>(c3f);
    register_vec_arithmetic<C3f,V3d>(c3f);
}

void
register_imath_vec3(py::module& m)
{
    // register all combinations of types, to support interoperability

    auto v3s = register_vec3<V3s>(m, "V3s");
    auto v3i = register_vec3<V3i>(m, "V3i");
    auto v3i64 = register_vec3<V3i64>(m, "V3i64");
    auto v3f = register_vec_fp3(register_vec3<V3f>(m, "V3f"));
    auto v3d = register_vec_fp3(register_vec3<V3d>(m, "V3d"));

    register_vec_arithmetic<V3s,V3s>(v3s);
    register_vec_arithmetic<V3s,V3i>(v3s);
    register_vec_arithmetic<V3s,V3i64>(v3s);
    register_vec_arithmetic<V3s,V3f>(v3s);
    register_vec_arithmetic<V3s,V3d>(v3s);

    register_vec_arithmetic<V3i,V3s>(v3i);
    register_vec_arithmetic<V3i,V3i>(v3i);
    register_vec_arithmetic<V3i,V3i64>(v3i);
    register_vec_arithmetic<V3i,V3f>(v3i);
    register_vec_arithmetic<V3i,V3d>(v3i);

    register_vec_arithmetic<V3i64,V3s>(v3i64);
    register_vec_arithmetic<V3i64,V3i>(v3i64);
    register_vec_arithmetic<V3i64,V3i64>(v3i64);
    register_vec_arithmetic<V3i64,V3f>(v3i64);
    register_vec_arithmetic<V3i64,V3d>(v3i64);

    register_vec_arithmetic<V3f,V3s>(v3f);
    register_vec_arithmetic<V3f,V3i>(v3f);
    register_vec_arithmetic<V3f,V3i64>(v3f);
    register_vec_arithmetic<V3f,V3f>(v3f);
    register_vec_arithmetic<V3f,V3d>(v3f);

    register_vec_arithmetic<V3d,V3s>(v3d);
    register_vec_arithmetic<V3d,V3i>(v3d);
    register_vec_arithmetic<V3d,V3i64>(v3d);
    register_vec_arithmetic<V3d,V3f>(v3d);
    register_vec_arithmetic<V3d,V3d>(v3d);
}

} // namespace PyBindImath


