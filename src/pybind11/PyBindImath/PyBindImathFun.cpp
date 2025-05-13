//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathFun.h>

namespace py = pybind11;

namespace {

void
register_fun(py::module& m)
{
    m.def("divs", IMATH_NAMESPACE::divs);
    m.def("mods", IMATH_NAMESPACE::mods);
    m.def("divp", IMATH_NAMESPACE::divp);
    m.def("modp", IMATH_NAMESPACE::modp);
    m.def("succf", IMATH_NAMESPACE::succf);
    m.def("predf", IMATH_NAMESPACE::predf);
    m.def("succd", IMATH_NAMESPACE::succd);
    m.def("predd", IMATH_NAMESPACE::predd);
    m.def("finitef", IMATH_NAMESPACE::finitef);
    m.def("finited", IMATH_NAMESPACE::finited);
}

template <class T>
void
register_fun_T(py::module& m)
{
    m.def("abs", IMATH_NAMESPACE::abs<T>);
    m.def("sign", IMATH_NAMESPACE::sign<T>);
    m.def("lerpfactor", IMATH_NAMESPACE::lerpfactor<T>);
    m.def("clamp", IMATH_NAMESPACE::clamp<T>);
    m.def("cmp", IMATH_NAMESPACE::cmp<T>);
    m.def("cmpt", IMATH_NAMESPACE::cmpt<T>);
    m.def("iszero", IMATH_NAMESPACE::iszero<T>);
    m.def("equal", IMATH_NAMESPACE::equal<T, T, T>);
    m.def("floor", IMATH_NAMESPACE::floor<T>);
    m.def("ceil", IMATH_NAMESPACE::ceil<T>);
    m.def("trunc", IMATH_NAMESPACE::trunc<T>);
}

template <class T, class Q>
void
register_fun_T_V(py::module& m)
{
    m.def("lerp", IMATH_NAMESPACE::lerp<T, Q>);
    m.def("ulerp", IMATH_NAMESPACE::ulerp<T, Q>);
}

} // namespace

namespace PyBindImath {

void
register_imath_fun(py::module& m)
{
    // Bindings for functions using explicit argument(s) and return types.
    register_fun(m);

    // Bindings for functions using a single template class (type) for its argument(s) and return types.
    register_fun_T<int>(m);
    register_fun_T<float>(m);
    register_fun_T<double>(m);

    // Bindings for functions using two template classes (types) for its argument(s) and return types.
    register_fun_T_V<int, int>(m);
    register_fun_T_V<int, float>(m);
    register_fun_T_V<int, double>(m);
    register_fun_T_V<float, int>(m);
    register_fun_T_V<float, float>(m);
    register_fun_T_V<float, double>(m);
    register_fun_T_V<double, int>(m);
    register_fun_T_V<double, float>(m);
    register_fun_T_V<double, double>(m);
}

} // namespace PyBindImath
