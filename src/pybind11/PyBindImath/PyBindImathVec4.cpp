//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"

//
// Wrappings for V4s, V4i, V4f, V4d
//
// The wrappings support inter-type operability, i.e:
//
//   v = V4i(1,2,3,4) + V2f(3,4,5,6)
//
// The wrappings also support constructors and operations between
// vectors and tuples and lists, i.e:
//
//   v = V4i((1,2,3,4))
//   v = V4i([1,2,3,4])
//   v = V4i(1,2,3,4) + (3,4,5,6)
//   v = V4i(1,2,3,4) + [3,4,5,6]
//

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Vec>
py::class_<Vec>
register_vec_fp4(py::class_<Vec> c)
{
    register_vec_mat<Vec,M44f>(c);
    register_vec_mat<Vec,M44d>(c);
    return register_vec_fp(c);
}

//
// Vec4<T>
//

template <class Vec>
py::class_<Vec>
register_vec4(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.def("__repr__", [name](const Vec& v) { return repr(name, v); })
        .def(py::init([](){return Vec(0);}))
        .def(py::init<short>())
        .def(py::init<short,short,short,short>())
        .def(py::init<int>())
        .def(py::init<int,int,int,int>())
        .def(py::init<float>())
        .def(py::init<float,float,float,float>())
        .def(py::init<double>())
        .def(py::init<double,double,double,double>())
        .def(py::init([](const py::tuple& o) { Vec v; return vecFromTuple<Vec>(o); }))
        .def(py::init([](const py::list& o) { Vec v; return vecFromList<Vec>(o); }))
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def_readwrite("z", &Vec::z)
        .def_readwrite("w", &Vec::z)
        .def("setValue", [](Vec& self, T x, T y, T z, T w) { self.setValue(x, y, z, w); }, "set to the given x,y,z,w values")
        ;

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

} // namespace

namespace PyBindImath {

void
register_imath_vec4(py::module& m)
{
    // register all combinations of types, to support interoperability

    auto v4s = register_vec4<V4s>(m, "V4s");
    auto v4i = register_vec4<V4i>(m, "V4i");
    auto v4i64 = register_vec4<V4i64>(m, "V4i64");
    auto v4f = register_vec_fp4(register_vec4<V4f>(m, "V4f"));
    auto v4d = register_vec_fp4(register_vec4<V4d>(m, "V4d"));

    register_vec_arithmetic<V4s,V4s>(v4s);
    register_vec_arithmetic<V4s,V4i>(v4s);
    register_vec_arithmetic<V4s,V4i64>(v4s);
    register_vec_arithmetic<V4s,V4f>(v4s);
    register_vec_arithmetic<V4s,V4d>(v4s);

    register_vec_arithmetic<V4i,V4s>(v4i);
    register_vec_arithmetic<V4i,V4i>(v4i);
    register_vec_arithmetic<V4i,V4i64>(v4i);
    register_vec_arithmetic<V4i,V4f>(v4i);
    register_vec_arithmetic<V4i,V4d>(v4i);

    register_vec_arithmetic<V4i64,V4s>(v4i64);
    register_vec_arithmetic<V4i64,V4i>(v4i64);
    register_vec_arithmetic<V4i64,V4i64>(v4i64);
    register_vec_arithmetic<V4i64,V4f>(v4i64);
    register_vec_arithmetic<V4i64,V4d>(v4i64);

    register_vec_arithmetic<V4f,V4s>(v4f);
    register_vec_arithmetic<V4f,V4i>(v4f);
    register_vec_arithmetic<V4f,V4i64>(v4f);
    register_vec_arithmetic<V4f,V4f>(v4f);
    register_vec_arithmetic<V4f,V4d>(v4f);

    register_vec_arithmetic<V4d,V4s>(v4d);
    register_vec_arithmetic<V4d,V4i>(v4d);
    register_vec_arithmetic<V4d,V4i64>(v4d);
    register_vec_arithmetic<V4d,V4f>(v4d);
    register_vec_arithmetic<V4d,V4d>(v4d);
}

} // namespace PyBindImath


