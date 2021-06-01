//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef PYIMATHEXPORT_H
#define PYIMATHEXPORT_H

#if defined(IMATH_DLL)
    #if defined(PLATFORM_VISIBILITY_AVAILABLE)
        #define PYIMATH_EXPORT __attribute__((visibility("default")))
        #define PYIMATH_EXPORT __attribute__((visibility("default")))
    #elif defined(_MSC_VER)
        #if defined(PYIMATH_BUILD)
            #define PYIMATH_EXPORT __declspec(dllexport)
        #else
            #define PYIMATH_EXPORT __declspec(dllimport)
        #endif
    #else
        #define PYIMATH_EXPORT
    #endif
#else
    #define PYIMATH_EXPORT
#endif

#endif // #ifndef PYIMATHEXPORT_H
