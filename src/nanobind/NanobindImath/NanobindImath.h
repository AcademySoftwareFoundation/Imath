//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include "NanobindImathExport.h"

#ifndef _NanobindImath_h_
#define _NanobindImath_h_

namespace NanobindImath {

NANOBINDIMATH_EXPORT void register_imath_vec(nanobind::module_& m);
NANOBINDIMATH_EXPORT void register_imath_box(nanobind::module_& m);

}

#endif
