//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathEuler.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

PYBIND11_MODULE(pybindimath, m)
{
    m.doc() = "PyBindImath module";
    m.attr("__version__") = IMATH_VERSION_STRING;

    PyBindImath::register_imath_vec2(m);
    PyBindImath::register_imath_vec3(m);
    PyBindImath::register_imath_vec4(m);
    PyBindImath::register_imath_matrix(m);
    PyBindImath::register_imath_box(m);
    PyBindImath::register_imath_color3(m);
    PyBindImath::register_imath_color4(m);
    PyBindImath::register_imath_euler(m);
    PyBindImath::register_imath_frustum(m);
    PyBindImath::register_imath_fun(m);
    PyBindImath::register_imath_line(m);
    PyBindImath::register_imath_plane(m);
    PyBindImath::register_imath_quat(m);
    PyBindImath::register_imath_random(m);
    PyBindImath::register_imath_shear(m);

    m.attr("INT_MIN")      = std::numeric_limits<int>::min();
    m.attr("INT_MAX")      = std::numeric_limits<int>::max();
    m.attr("INT_LOWEST")   = std::numeric_limits<int>::lowest();
    m.attr("INT_EPS")      = std::numeric_limits<int>::epsilon();

    m.attr("FLT_MIN")      = std::numeric_limits<float>::min();
    m.attr("FLT_MAX")      = std::numeric_limits<float>::max();
    m.attr("FLT_LOWEST")   = std::numeric_limits<float>::lowest();
    m.attr("FLT_EPS")      = std::numeric_limits<float>::epsilon();

    m.attr("DBL_MIN")      = std::numeric_limits<double>::min();
    m.attr("DBL_MAX")      = std::numeric_limits<double>::max();
    m.attr("DBL_LOWEST")   = std::numeric_limits<double>::lowest();
    m.attr("DBL_EPS")      = std::numeric_limits<double>::epsilon();

}
