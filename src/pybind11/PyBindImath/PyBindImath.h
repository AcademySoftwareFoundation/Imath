//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include "PyBindImathExport.h"

#ifndef _PyBindImath_h_
#define _PyBindImath_h_

namespace PyBindImath {

PYBINDIMATH_EXPORT void register_imath_box(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_color3(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_color4(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_euler(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_frustum(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_fun(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_line(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_matrix(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_plane(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_quat(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_random(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_shear(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_vec2(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_vec3(pybind11::module& m);
PYBINDIMATH_EXPORT void register_imath_vec4(pybind11::module& m);

} // namespace PyBindImath

#endif
