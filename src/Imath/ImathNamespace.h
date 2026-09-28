//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// The Imath library namespace
//
// The purpose of this file is to make it possible to specify an
// IMATH_NAMESPACE as a preprocessor definition and have all of the
// Imath symbols defined within that namespace rather than the
// standard Imath namespace. Those symbols are made available to
// client code through the IMATH_INTERNAL_NAMESPACE or the
// IMATH_STABLE_* namespaces in addition to the IMATH_NAMESPACE.
//
// To ensure source code compatibility, the IMATH_NAMESPACE defaults to Imath
// and then "using namespace IMATH_INTERNAL_NAMESPACE;" brings all of the
// declarations from the IMATH_INTERNAL_NAMESPACE into the IMATH_NAMESPACE.
// This means that client code can continue to use syntax like Imath::V3f,
// but at link time it will resolve to a mangled symbol based on the
// IMATH_INTERNAL_NAMESPACE.
//
// As an example, if one needed to build against a newer version of Imath and
// have it run alongside an older version in the same application, it is now
// possible to use an internal namespace to prevent collisions between the
// older versions of Imath symbols and the newer ones.  To do this, the
// following could be defined at build time:
//
// IMATH_INTERNAL_NAMESPACE = Imath_v2
//
// This means that declarations inside Imath headers look like this (after
// the preprocessor has done its work):
//
// namespace Imath_v2 {
//     ...
//     class declarations
//     ...
// }
//
// namespace Imath {
//     using namespace Imath_v2;
// }
//
// The symbols the library have been defined as stable. These no
// longer have a truly configurable name simply via
// IMATH_INTERNAL_NAMESPACE. Instead, the top level namespace is still
// allowed to be customized, but the stable version number is applied
// as a fixed function onto the INTERNAL name. This means you can
// define a custom name, such as MyImath, and all the symbols will not
// conflict with system / default builds. But a default build will
// have a consistent ABI.
//
// If you set the IMATH_INTERNAL_NAMESPACE to MyImath, the stable
// symbols would then have a symbol name such as MyImath_3_2::V3f. But
// if you don't override the top level IMATH_NAMESPACE, they could be
// (in code using the custom build) be accessed as Imath::V3f, but
// default to your version of the stable symbols.
//

#ifndef INCLUDED_IMATHNAMESPACE_H
#define INCLUDED_IMATHNAMESPACE_H

/// @cond Doxygen_Suppress

#include "ImathConfig.h"

#ifndef IMATH_NAMESPACE
#    define IMATH_NAMESPACE Imath
#endif

#ifndef IMATH_INTERNAL_NAMESPACE
#    define IMATH_INTERNAL_NAMESPACE IMATH_NAMESPACE
#endif

#ifdef __cplusplus

// usually, one does abi versioning with nested namespaces, where the
// version number is in the inner one and using inline namespaces to
// elevate the visiblity but otherwise keep the nesting, but imath has
// had two top level namespaces: one versioned, one an alias in order
// to provide customization while still retaining versioned / "stable"
// ones so we keep the same semantics of importing the internal /
// stables ones into the public one

#define IMATH_STABLE_NS_HELPER(base,maj,min) base ## _ ## maj ## _ ## min
#define IMATH_STABLE_NS_VER(base,maj,min) IMATH_STABLE_NS_HELPER(base,maj,min)
#define IMATH_STABLE_3_2_NAME IMATH_STABLE_NS_VER(IMATH_INTERNAL_NAMESPACE,3,2)

namespace IMATH_INTERNAL_NAMESPACE {
}
namespace IMATH_STABLE_3_2_NAME {
}

namespace IMATH_NAMESPACE {
// pull in both namespaces to the top level imath namespace but with
// using so the original stays as the symbol name
using namespace IMATH_INTERNAL_NAMESPACE;
using namespace IMATH_STABLE_3_2_NAME;
} // namespace IMATH_NAMESPACE

#define IMATH_INTERNAL_NS_ENTER namespace IMATH_INTERNAL_NAMESPACE {
#define IMATH_INTERNAL_NS_EXIT }

// Stable ABI macros - these should be used to define
// classes/functions with a consistent version name
#define IMATH_STABLE_3_2_ENTER namespace IMATH_STABLE_3_2_NAME {
#define IMATH_STABLE_3_2_EXIT }

#endif // __cplusplus

/// @endcond

#endif /* INCLUDED_IMATHNAMESPACE_H */
