//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "halfLimits.h"
#include <assert.h>
#include <cmath>
#include <iostream>
#include "testLimits.h"

using namespace std;

namespace
{

float
mypow (int x, int y)
{
    bool negative = false;

    if (y < 0)
    {
        negative = true;
        y        = -y;
    }

    float z = 1;

    while (y > 0)
    {
        z *= x;
        y -= 1;
    }

    if (negative)
        z = 1 / z;

    return z;
}

} // namespace

void
testLimits()
{
    cout << "values in std::numeric_limits<half>\n";

    cout << "min_exponent\n";

    {
        half h (mypow (2, numeric_limits<half>::min_exponent - 1));
        assert (h.isNormalized());
    }

    {
        half h (mypow (2, numeric_limits<half>::min_exponent - 2));
        assert (h.isDenormalized());
    }

    cout << "max_exponent\n";

    {
        half h (mypow (2, numeric_limits<half>::max_exponent - 1));
        assert (h.isNormalized());
    }

    {
        half h (mypow (2, numeric_limits<half>::max_exponent));
        assert (h.isInfinity());
    }

    cout << "min_exponent10\n";

    {
        half h (mypow (10, numeric_limits<half>::min_exponent10));
        assert (h.isNormalized());
    }

    {
        half h (mypow (10, numeric_limits<half>::min_exponent10 - 1));
        assert (h.isDenormalized());
    }

    cout << "max_exponent10\n";

    {
        half h (mypow (10, numeric_limits<half>::max_exponent10));
        assert (h.isNormalized());
    }

    {
        half h (mypow (10, numeric_limits<half>::max_exponent10 + 1));
        assert (h.isInfinity());
    }

#if __cplusplus >= 201103L

    cout << "max_digits10\n";
    assert (numeric_limits<half>::max_digits10 ==
            std::ceil (numeric_limits<half>::digits * std::log10 (2) + 1));

    cout << "lowest\n";
    assert (numeric_limits<half>::lowest() == -HALF_MAX);

#endif

    cout << "ok\n\n" << flush;
}

void
testHalfLimits()
{
    cout << "values in std::numeric_limits<half>\n";

    // For reference:
    printf("HALF_DENORM_MIN %g -> 0x%04x\n", (float)HALF_DENORM_MIN, half(HALF_DENORM_MIN).bits());
    printf("HALF_NRM_MIN %g -> 0x%04x\n", (float)HALF_NRM_MIN, half(HALF_NRM_MIN).bits());
    printf("HALF_MIN %g -> 0x%04x\n", (float)HALF_MIN, half(HALF_MIN).bits());
    printf("HALF_MAX %g -> 0x%04x\n", (float)HALF_MAX, half(HALF_MAX).bits());
    printf("HALF_LOWEST %g -> 0x%04x\n", (float)-HALF_MAX, half(-HALF_MAX).bits());
    printf("HALF_EPSILON %g -> 0x%04x\n", (float)HALF_EPSILON, half(HALF_EPSILON).bits());
    printf("half posInf %g -> 0x%04x\n", (float)half::posInf(), half::posInf().bits());
    printf("half negInf %g -> 0x%04x\n", (float)half::negInf(), half::negInf().bits());
    printf("half qNan %g -> 0x%04x\n", (float)half::qNan(), half::qNan().bits());
    printf("half sNan %g -> 0x%04x\n", (float)half::sNan(), half::sNan().bits());
    printf("numeric_limits<half> min %g -> 0x%04x\n", (float)std::numeric_limits<half>::min(), std::numeric_limits<half>::min().bits());
    printf("numeric_limits<half> max %g -> 0x%04x\n", (float)std::numeric_limits<half>::max(), std::numeric_limits<half>::max().bits());
    printf("numeric_limits<half> lowest %g -> 0x%04x\n", (float)std::numeric_limits<half>::lowest(), std::numeric_limits<half>::lowest().bits());
    printf("numeric_limits<half> epsilon %g -> 0x%04x\n", (float)std::numeric_limits<half>::epsilon(), std::numeric_limits<half>::epsilon().bits());
    printf("numeric_limits<half> round_error %g -> 0x%04x\n", (float)std::numeric_limits<half>::round_error(), std::numeric_limits<half>::round_error().bits());
    printf("numeric_limits<half> infinity %g -> 0x%04x\n", (float)std::numeric_limits<half>::infinity(), std::numeric_limits<half>::infinity().bits());
    printf("numeric_limits<half> quiet_NaN %g -> 0x%04x\n", (float)std::numeric_limits<half>::quiet_NaN(), std::numeric_limits<half>::quiet_NaN().bits());
    printf("numeric_limits<half> signaling_NaN %g -> 0x%04x\n", (float)std::numeric_limits<half>::signaling_NaN(), std::numeric_limits<half>::signaling_NaN().bits());
    printf("numeric_limits<half> denorm_min %g -> 0x%04x\n", (float)std::numeric_limits<half>::denorm_min(), std::numeric_limits<half>::denorm_min().bits());

    assert (std::numeric_limits<half>::max() == half(HALF_MAX));
    assert (std::numeric_limits<half>::min() == half(HALF_NRM_MIN));
    assert (std::numeric_limits<half>::denorm_min() == half(HALF_DENORM_MIN));
    assert (std::numeric_limits<half>::lowest() == half(-HALF_MAX));
    assert (std::numeric_limits<half>::epsilon() == half(HALF_EPSILON));
    assert (std::numeric_limits<half>::infinity() == half::posInf());
    assert (std::numeric_limits<half>::quiet_NaN().bits() == half::qNan().bits());
    assert (std::numeric_limits<half>::signaling_NaN().bits() == half::sNan().bits());
    assert (std::numeric_limits<half>::infinity() == half::posInf());

    cout << "ok\n\n" << flush;
}
