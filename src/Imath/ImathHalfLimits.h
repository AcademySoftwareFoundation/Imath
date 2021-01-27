//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

///
/// @file ImathHalfLimits.h
///
/// @brief Imath-style limits for class half.
///

#ifndef INCLUDED_IMATHHALFLIMITS_H
#define INCLUDED_IMATHHALFLIMITS_H

#include "ImathLimits.h"
#include "ImathNamespace.h"

#include "half.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

///
/// class of static methods return limit values of type half.
///

template <> struct limits<half>
{
    /// Largest possible negative value
    IMATH_HOSTDEVICE static constexpr float min() noexcept { return -HALF_MAX; }

    /// Largest possible positive value
    IMATH_HOSTDEVICE static constexpr float max() noexcept { return HALF_MAX; }

    /// Smallest possible positive value
    IMATH_HOSTDEVICE static constexpr float smallest() noexcept { return HALF_MIN; }

    /// Smallest possible e for which 1+e != 1
    IMATH_HOSTDEVICE static constexpr float epsilon() noexcept { return HALF_EPSILON; }

    /// Return false: half is not integral.
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return false; }
    /// Return true: half is signed.
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHHALFLIMITS_H
