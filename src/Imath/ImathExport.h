//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// See the document in OpenEXR docs/SymbolVisibility.md for background
// on all these types
#if defined(IMATH_DLL)
#    if defined(IMATH_EXPORTS)
#        define IMATH_EXPORT __declspec(dllexport)
#        define IMATH_EXPORT_CONST extern __declspec(dllexport)
//#        if defined(__MINGW32__)
//#            define IMATH_EXPORT_EXTERN_TEMPLATE IMATH_EXPORT
//#        else
//#            define IMATH_EXPORT_TEMPLATE_INSTANCE IMATH_EXPORT
//#        endif
#    else
#        define IMATH_EXPORT __declspec(dllimport)
#        define IMATH_EXPORT_CONST extern __declspec(dllimport)
#    endif
#else
#    ifndef _MSC_VER
#        define IMATH_EXPORT __attribute__ ((__visibility__ ("default")))
#        define IMATH_EXPORT_TYPE __attribute__ ((__visibility__ ("default")))
//#        define IMATH_EXPORT_TEMPLATE_DATA __attribute__ ((__visibility__ ("default")))
#        define IMATH_HIDDEN __attribute__ ((__visibility__ ("hidden")))
#        if __has_attribute(__type_visibility__)
#            define IMATH_EXPORT_ENUM __attribute__ ((__type_visibility__ ("default")))
#            define IMATH_EXPORT_TEMPLATE_TYPE __attribute__ ((__type_visibility__ ("default")))
//#            define IMATH_EXPORT_EXTERN_TEMPLATE __attribute__ ((__visibility__ ("default")))
#        else
#            define IMATH_EXPORT_TEMPLATE_TYPE __attribute__ ((__visibility__ ("default")))
#        endif
#        define IMATH_EXPORT_CONST extern const __attribute__ ((visibility ("default")))
#    else
#	     define IMATH_EXPORT
#        define IMATH_EXPORT_CONST extern const
#    endif
#endif
#ifndef IMATH_EXPORT_TYPE
#    define IMATH_EXPORT_TYPE
#endif
#ifndef IMATH_EXPORT_TEMPLATE_TYPE
#    define IMATH_EXPORT_TEMPLATE_TYPE
#endif
//#ifndef IMATH_EXPORT_EXTERN_TEMPLATE
//#    define IMATH_EXPORT_EXTERN_TEMPLATE
//#endif
//#ifndef IMATH_EXPORT_TEMPLATE_INSTANCE
//#    define IMATH_EXPORT_TEMPLATE_INSTANCE
//#endif
//#ifndef IMATH_EXPORT_TEMPLATE_TYPE
//#    define IMATH_EXPORT_TEMPLATE_TYPE
//#endif
#ifndef IMATH_EXPORT_ENUM
#    define IMATH_EXPORT_ENUM
#endif
#ifndef IMATH_HIDDEN
#    define IMATH_HIDDEN
#endif
