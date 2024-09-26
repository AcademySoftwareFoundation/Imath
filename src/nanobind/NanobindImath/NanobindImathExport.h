//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef NANOBINDIMATHEXPORT_H
#define NANOBINDIMATHEXPORT_H

#if defined(IMATH_DLL)
    #if defined(PLATFORM_VISIBILITY_AVAILABLE)
        #define NANOBINDIMATH_EXPORT __attribute__((visibility("default")))
        #define NANOBINDIMATH_EXPORT __attribute__((visibility("default")))
    #elif defined(_MSC_VER)
        #if defined(NANOBINDIMATH_BUILD)
            #define NANOBINDIMATH_EXPORT __declspec(dllexport)
        #else
            #define NANOBINDIMATH_EXPORT __declspec(dllimport)
        #endif
    #else
        #define NANOBINDIMATH_EXPORT
    #endif
#else
    #define NANOBINDIMATH_EXPORT
#endif

#endif // #ifndef NANOBINDIMATHEXPORT_H
