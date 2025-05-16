//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathFun.h>

namespace py = pybind11;

namespace {

template <class T>
void
register_imath_funT(py::module& m)
{
    m.def("cmp", IMATH_NAMESPACE::cmp<T>);
    m.def("cmpt", IMATH_NAMESPACE::cmpt<T>);
    m.def("iszero", IMATH_NAMESPACE::iszero<T>);
    m.def("equal", IMATH_NAMESPACE::equal<T, T, T>);

}

} // namespace 

namespace PyBindImath {

void
register_imath_fun(py::module& m)
{
    register_imath_funT<int>(m);
    register_imath_funT<float>(m);
    register_imath_funT<double>(m);
}

} // namespace PyBindImath

