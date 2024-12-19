//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathEuler.h>

PYBIND11_MODULE(pybindimath, m)
{
    m.doc() = "PyBindImath module";
    m.attr("__version__") = IMATH_VERSION_STRING;

    PyBindImath::register_imath_vec(m);
    PyBindImath::register_imath_box(m);
    PyBindImath::register_imath_plane(m);
    PyBindImath::register_imath_line(m);
    // PyBindImath::register_imath_euler(m)
    // PyBindImath::register_imath_frustum(m)


    //
    // Initialize constants
    //
    pybind11::enum_<IMATH_NAMESPACE::Eulerf::Order>(m, "Order")
        .value("EULER_XYZ", IMATH_NAMESPACE::Eulerf::XYZ)
        .value("EULER_XZY", IMATH_NAMESPACE::Eulerf::XZY)
        .value("EULER_YZX", IMATH_NAMESPACE::Eulerf::YZX)
        .value("EULER_YXZ", IMATH_NAMESPACE::Eulerf::YXZ)
        .value("EULER_ZXY", IMATH_NAMESPACE::Eulerf::ZXY)
        .value("EULER_ZYX", IMATH_NAMESPACE::Eulerf::ZYX)
        .value("EULER_XZX", IMATH_NAMESPACE::Eulerf::XZX)
        .value("EULER_XYX", IMATH_NAMESPACE::Eulerf::XYX)
        .value("EULER_YXY", IMATH_NAMESPACE::Eulerf::YXY)
        .value("EULER_YZY", IMATH_NAMESPACE::Eulerf::YZY)
        .value("EULER_ZYZ", IMATH_NAMESPACE::Eulerf::ZYZ)
        .value("EULER_ZXZ", IMATH_NAMESPACE::Eulerf::ZXZ)
        .value("EULER_XYZr", IMATH_NAMESPACE::Eulerf::XYZr)
        .value("EULER_XZYr", IMATH_NAMESPACE::Eulerf::XZYr)
        .value("EULER_YZXr", IMATH_NAMESPACE::Eulerf::YZXr)
        .value("EULER_YXZr", IMATH_NAMESPACE::Eulerf::YXZr)
        .value("EULER_ZXYr", IMATH_NAMESPACE::Eulerf::ZXYr)
        .value("EULER_ZYXr", IMATH_NAMESPACE::Eulerf::ZYXr)
        .value("EULER_XZXr", IMATH_NAMESPACE::Eulerf::XZXr)
        .value("EULER_XYXr", IMATH_NAMESPACE::Eulerf::XYXr)
        .value("EULER_YXYr", IMATH_NAMESPACE::Eulerf::YXYr)
        .value("EULER_YZYr", IMATH_NAMESPACE::Eulerf::YZYr)
        .value("EULER_ZYZr", IMATH_NAMESPACE::Eulerf::ZYZr)
        .value("EULER_ZXZr", IMATH_NAMESPACE::Eulerf::ZXZr)
        .export_values();

    pybind11::enum_<IMATH_NAMESPACE::Eulerf::Axis>(m, "Axis")
        .value("EULER_X_AXIS", IMATH_NAMESPACE::Eulerf::X)
        .value("EULER_Y_AXIS", IMATH_NAMESPACE::Eulerf::Y)
        .value("EULER_Z_AXIS", IMATH_NAMESPACE::Eulerf::Z)
        .export_values();

    pybind11::enum_<IMATH_NAMESPACE::Eulerf::InputLayout>(m, "InputLayout")
        .value("EULER_IJKLayout", IMATH_NAMESPACE::Eulerf::IJKLayout)
        .value("EULER_XYZLayout", IMATH_NAMESPACE::Eulerf::XYZLayout)
        .export_values();


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
