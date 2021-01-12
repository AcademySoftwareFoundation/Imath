//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#if defined(IMATH_DLL)
#    if defined(IMATH_EXPORTS)
#        define IMATH_EXPORT __declspec(dllexport)
#        define IMATH_EXPORT_CONST extern __declspec(dllexport)
#    else
#        define IMATH_EXPORT __declspec(dllimport)
#        define IMATH_EXPORT_CONST extern __declspec(dllimport)
#    endif
#else
#    define IMATH_EXPORT
#    define IMATH_EXPORT_CONST extern const
#endif
