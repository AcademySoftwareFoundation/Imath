//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef PYBINDIMATHEXPORT_H
#define PYBINDIMATHEXPORT_H

#if defined(IMATH_DLL)
    #if defined(PLATFORM_VISIBILITY_AVAILABLE)
        #define PYBINDIMATH_EXPORT __attribute__((visibility("default")))
        #define PYBINDIMATH_EXPORT __attribute__((visibility("default")))
    #elif defined(_MSC_VER)
        #if defined(PYBINDIMATH_BUILD)
            #define PYBINDIMATH_EXPORT __declspec(dllexport)
        #else
            #define PYBINDIMATH_EXPORT __declspec(dllimport)
        #endif
    #else
        #define PYBINDIMATH_EXPORT
    #endif
#else
    #define PYBINDIMATH_EXPORT
#endif

#endif // #ifndef PYBINDIMATHEXPORT_H
