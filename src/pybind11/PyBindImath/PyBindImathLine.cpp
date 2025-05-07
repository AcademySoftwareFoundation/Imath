//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathLine.h>

namespace {

template <class T, class Q, class S>
void register_line(pybind11::module& m, const char *name)
{
    pybind11::class_<T> c(m, name);    
    c.def(pybind11::init<>(), "Uninitialized by default")
    .def(pybind11::init<const Q &, Q &>(), pybind11::arg("point1"), pybind11::arg("point2"), "Initialize with two points. The direction is the difference between the points.")
    .def("__str__", [](const T &obj) {
        std::stringstream ss;
        ss << obj;
        return ss.str();
        });
}

} // namespace
 
namespace PyBindImath {
 
void
register_imath_line(pybind11::module &m) 
{
    register_line<IMATH_NAMESPACE::Line3f, IMATH_NAMESPACE::V3f, float>(m, "Line3f");
    register_line<IMATH_NAMESPACE::Line3d, IMATH_NAMESPACE::V3d, double>(m, "Line3d");
}

} // namespace PyBindImath

