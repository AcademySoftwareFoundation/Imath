//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHEXPORT_H
#define INCLUDED_IMATHEXPORT_H

/// \defgroup ExportMacros Macros to manage symbol visibility
///
/// There is more information about the motivation for these macros
/// documented in the OpenEXR source tree
/// (https://github.com/AcademySoftwareFoundation/openexr) under
/// docs/SymbolVisibility.md
///
/// Imath only needs a couple of the possible macros outlined in the
/// above document, and due to it largely being inline only, does not
/// have much to do.
/// 
/// @{
#if defined(IMATH_DLL)

// when building Imath as a DLL for Windows, we have to control the
// typical DLL export / import things. Luckily, the typeinfo is all
// automatic there, so only have to deal with symbols, except Windows
// has some weirdness with DLLs and extern const, so we have to
// provide a macro to handle that.

#  if defined(IMATH_EXPORTS)
#    define IMATH_EXPORT __declspec(dllexport)
#    define IMATH_EXPORT_CONST extern __declspec(dllexport)
#  else
#    define IMATH_EXPORT __declspec(dllimport)
#    define IMATH_EXPORT_CONST extern __declspec(dllimport)
#  endif

#else

// Need to handle when we are building as a static library under
// windows and with a compiler that doesn't understand visibility
// attributes.
#  ifndef _MSC_VER
// Allow the user to override and not control visibility
#    ifdef IMATH_USE_DEFAULT_VISIBILITY
#      define IMATH_EXPORT
#      define IMATH_EXPORT_CONST extern const
// else We are using a compiler that knows about visibility, so works for both
// shared and static libraries...
#    else // IMATH_USE_DEFAULT_VISIBILITY
#      define IMATH_EXPORT __attribute__ ((__visibility__ ("default")))
#      define IMATH_EXPORT_TYPE __attribute__ ((__visibility__ ("default")))
#      define IMATH_HIDDEN __attribute__ ((__visibility__ ("hidden")))
// clang differs from gcc and has type visibility which is needed for enums and templates
#      if __has_attribute(__type_visibility__)
#        define IMATH_EXPORT_ENUM __attribute__ ((__type_visibility__ ("default")))
#        define IMATH_EXPORT_TEMPLATE_TYPE __attribute__ ((__type_visibility__ ("default")))
#      else
#        define IMATH_EXPORT_TEMPLATE_TYPE __attribute__ ((__visibility__ ("default")))
#      endif
#      define IMATH_EXPORT_CONST extern const __attribute__ ((visibility ("default")))
#    endif
#  else // _MSC_VER
#    define IMATH_EXPORT
#    define IMATH_EXPORT_CONST extern const
#  endif

#endif // IMATH_DLL

// Provide defaults so we don't have to replicate lines as much
#ifndef IMATH_EXPORT_TYPE
#    define IMATH_EXPORT_TYPE
#endif
#ifndef IMATH_EXPORT_TEMPLATE_TYPE
#    define IMATH_EXPORT_TEMPLATE_TYPE
#endif
#ifndef IMATH_EXPORT_ENUM
#    define IMATH_EXPORT_ENUM
#endif
#ifndef IMATH_HIDDEN
#    define IMATH_HIDDEN
#endif

/// @}

#endif // INCLUDED_IMATHEXPORT_H
