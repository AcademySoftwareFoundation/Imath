//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathFixedArray.h"
#include "PyImathExport.h"

namespace PyImath {

template <> PYIMATH_EXPORT bool FixedArrayDefaultValue<bool>::value() { return false; }
template <> PYIMATH_EXPORT signed char FixedArrayDefaultValue<signed char>::value() { return 0; }
template <> PYIMATH_EXPORT unsigned char FixedArrayDefaultValue<unsigned char>::value() { return 0; }
template <> PYIMATH_EXPORT short FixedArrayDefaultValue<short>::value() { return 0; }
template <> PYIMATH_EXPORT unsigned short FixedArrayDefaultValue<unsigned short>::value() { return 0; }
template <> PYIMATH_EXPORT int FixedArrayDefaultValue<int>::value() { return 0; }
template <> PYIMATH_EXPORT int64_t FixedArrayDefaultValue<int64_t>::value() { return 0; }
template <> PYIMATH_EXPORT unsigned int FixedArrayDefaultValue<unsigned int>::value() { return 0; }
template <> PYIMATH_EXPORT float FixedArrayDefaultValue<float>::value() { return 0; }
template <> PYIMATH_EXPORT double FixedArrayDefaultValue<double>::value() { return 0; }

//int alloc_count = 0;

}
