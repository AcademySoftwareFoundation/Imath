///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

// Primary authors:
//     Florian Kainz <kainz@ilm.com>
//     Rod Bogart <rgb@ilm.com>

#ifndef INCLUDED_HALF_LIMITS_H
#define INCLUDED_HALF_LIMITS_H

//------------------------------------------------------------------------
//
//	C++ standard library-style numeric_limits for class half
//
//------------------------------------------------------------------------

#include "half.h"
#include <limits>

namespace std
{

template <> class numeric_limits<half>
{
  public:
    static const bool is_specialized = true;

    static /*constexpr*/ half min() noexcept { return HALF_NRM_MIN; }
    static /*constexpr*/ half max() noexcept { return HALF_MAX; }
    static /*constexpr*/ half lowest() { return -HALF_MAX; }

    static constexpr int digits       = HALF_MANT_DIG;
    static constexpr int digits10     = HALF_DIG;
    static constexpr int max_digits10 = HALF_DECIMAL_DIG;
    static constexpr bool is_signed   = true;
    static constexpr bool is_integer  = false;
    static constexpr bool is_exact    = false;
    static constexpr int radix        = HALF_RADIX;
    static /*constexpr*/ half epsilon() noexcept { return HALF_EPSILON; }
    static /*constexpr*/ half round_error() noexcept { return HALF_EPSILON / 2; }

    static constexpr int min_exponent   = HALF_MIN_EXP;
    static constexpr int min_exponent10 = HALF_MIN_10_EXP;
    static constexpr int max_exponent   = HALF_MAX_EXP;
    static constexpr int max_exponent10 = HALF_MAX_10_EXP;

    static constexpr bool has_infinity             = true;
    static constexpr bool has_quiet_NaN            = true;
    static constexpr bool has_signaling_NaN        = true;
    static constexpr float_denorm_style has_denorm = denorm_present;
    static constexpr bool has_denorm_loss          = false;
    static /*constexpr*/ half infinity() noexcept { return half::posInf(); }
    static /*constexpr*/ half quiet_NaN() noexcept { return half::qNan(); }
    static /*constexpr*/ half signaling_NaN() noexcept { return half::sNan(); }
    static /*constexpr*/ half denorm_min() noexcept { return HALF_MIN; }

    static constexpr bool is_iec559  = false;
    static constexpr bool is_bounded = false;
    static constexpr bool is_modulo  = false;

    static constexpr bool traps                    = true;
    static constexpr bool tinyness_before          = false;
    static constexpr float_round_style round_style = round_to_nearest;

    // FIXME: These methods should all be constexpr, but many of them are
    // currently not able to be because half is not completedly plumbed for
    // constexpr (and my not be able to be in C++11).
};

} // namespace std

#endif
