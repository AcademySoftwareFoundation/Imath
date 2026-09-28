//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "ImathFun.h"
#include <cmath>

IMATH_STABLE_3_2_ENTER

float succf(float f) IMATH_NOEXCEPT
{
    return std::isfinite(f) ?
            std::nextafter(f, std::numeric_limits<float>::infinity()) : f;
}

float predf(float f) IMATH_NOEXCEPT
{
    return std::isfinite(f) ?
            std::nextafter(f, -std::numeric_limits<float>::infinity()) : f;
}

double succd(double d) IMATH_NOEXCEPT
{
    return std::isfinite(d) ?
            std::nextafter(d, std::numeric_limits<double>::infinity()) : d;
}

double predd(double d) IMATH_NOEXCEPT
{
    return  std::isfinite(d) ?
            std::nextafter(d, -std::numeric_limits<double>::infinity()) : d;
}

IMATH_STABLE_3_2_EXIT
