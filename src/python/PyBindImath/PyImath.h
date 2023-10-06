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
#include "PyImathExport.h"

#ifndef _PyBindImath_h_
#define _PyBindImath_h_

namespace PyImath {

PYIMATH_EXPORT void register_imath_vec(pybind11::module& m);

}

#endif
