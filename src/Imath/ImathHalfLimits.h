//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHHALFLIMITS_H
#define INCLUDED_IMATHHALFLIMITS_H

//--------------------------------------------------
//
//	Imath-style limits for class half.
//
//--------------------------------------------------

#include "ImathLimits.h"
#include "ImathNamespace.h"

#include "half.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

template <> struct limits<half>
{
    IMATH_HOSTDEVICE static constexpr float min() noexcept { return -HALF_MAX; }
    IMATH_HOSTDEVICE static constexpr float max() noexcept { return HALF_MAX; }
    IMATH_HOSTDEVICE static constexpr float smallest() noexcept { return HALF_MIN; }
    IMATH_HOSTDEVICE static constexpr float epsilon() noexcept { return HALF_EPSILON; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return false; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHHALFLIMITS_H
