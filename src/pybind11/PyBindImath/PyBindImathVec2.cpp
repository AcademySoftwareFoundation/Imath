//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"
#include <ImathVec.h>
#include <ImathVecAlgo.h>
#include <ImathMatrix.h>

//
// Wrappings for V2s, V2i, V2f, V2d
//
// The wrappings support inter-type operability, i.e:
//
//   v = V2i(1,2) + V2f(3,4)
//
// The wrappings also support constructors and operations between
// vectors and tuples and lists, i.e:
//
//   v = V2i((1,2))
//   v = V2i([1,2])
//   v = V2i(1,2) + (3,4)
//   v = V2i(1,2) + [3,4]
//

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Vec>
py::class_<Vec>
register_vec_fp2(py::class_<Vec> c)
{
    register_vec_mat<Vec,M22f>(c);
    register_vec_mat<Vec,M22d>(c);
    register_vec_mat<Vec,M33f>(c);
    register_vec_mat<Vec,M33d>(c);
    return register_vec_fp(c);
}

//
// Vec2<T>
//

template <class Vec>
py::class_<Vec>
register_vec2(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.attr("__module__") = "";
    c.def("__repr__", [name](const Vec& v) { return repr(name, v); })
        .def(py::init([](){return Vec(0);}))
        .def(py::init<short>())
        .def(py::init<short,short>())
        .def(py::init<int>())
        .def(py::init<int,int>())
        .def(py::init<float>())
        .def(py::init<double>())
        .def(py::init<float,float>())
        .def(py::init<double,double>())
        .def(py::init([](const py::tuple& t) { return vecFromTuple<Vec>(t); }))
        .def(py::init([](const py::list& l) { return vecFromList<Vec>(l); }))
        .def(py::self % py::self)
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def("setValue", [](Vec& self, T x, T y) { self.setValue(x, y); }, "set to the given x,y values")
        .def("cross", &Vec::cross, "return the right-handed cross product with the given vector")

        ;

    register_vec_geom<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

} // namespace

namespace PyBindImath {

void
register_imath_vec2(py::module& m)
{
    // register all combinations of types, to support interoperability

    auto v2s = register_vec2<V2s>(m, "V2s");
    auto v2i = register_vec2<V2i>(m, "V2i");
    auto v2i64 = register_vec2<V2i64>(m, "V2i64");
    auto v2f = register_vec_fp2(register_vec2<V2f>(m, "V2f"));
    auto v2d = register_vec_fp2(register_vec2<V2d>(m, "V2d"));

    register_vec_arithmetic<V2s,V2s>(v2s);
    register_vec_arithmetic<V2s,V2i>(v2s);
    register_vec_arithmetic<V2s,V2i64>(v2s);
    register_vec_arithmetic<V2s,V2f>(v2s);
    register_vec_arithmetic<V2s,V2d>(v2s);

    register_vec_arithmetic<V2i,V2s>(v2i);
    register_vec_arithmetic<V2i,V2i>(v2i);
    register_vec_arithmetic<V2i,V2i64>(v2i);
    register_vec_arithmetic<V2i,V2f>(v2i);
    register_vec_arithmetic<V2i,V2d>(v2i);

    register_vec_arithmetic<V2i64,V2s>(v2i64);
    register_vec_arithmetic<V2i64,V2i>(v2i64);
    register_vec_arithmetic<V2i64,V2i64>(v2i64);
    register_vec_arithmetic<V2i64,V2f>(v2i64);
    register_vec_arithmetic<V2i64,V2d>(v2i64);

    register_vec_arithmetic<V2f,V2s>(v2f);
    register_vec_arithmetic<V2f,V2i>(v2f);
    register_vec_arithmetic<V2f,V2i64>(v2f);
    register_vec_arithmetic<V2f,V2f>(v2f);
    register_vec_arithmetic<V2f,V2d>(v2f);

    register_vec_arithmetic<V2d,V2s>(v2d);
    register_vec_arithmetic<V2d,V2i>(v2d);
    register_vec_arithmetic<V2d,V2i64>(v2d);
    register_vec_arithmetic<V2d,V2f>(v2d);
    register_vec_arithmetic<V2d,V2d>(v2d);
}

} // namespace PyBindImath


