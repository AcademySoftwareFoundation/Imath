//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef PYBINDIMATHEXPORT_H
#define PYBINDIMATHEXPORT_H

#if defined(_MSC_VER)
    #if defined(IMATH_DLL)
        #if defined(PYBINDIMATH_BUILD)
            #define PYBINDIMATH_EXPORT __declspec(dllexport)
        #else
            #define PYBINDIMATH_EXPORT __declspec(dllimport)
        #endif
    #else
        #define PYBINDIMATH_EXPORT
    #endif
#else
    #define PYBINDIMATH_EXPORT __attribute__((visibility("default")))
#endif


#endif // #ifndef PYBINDIMATHEXPORT_H
