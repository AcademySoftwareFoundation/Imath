//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathFunOperators_h_
#define _PyImathFunOperators_h_

#include <ImathVec.h>
#include <ImathMatrixAlgo.h>
#include <ImathColorAlgo.h>
#include <ImathFun.h>
#include <cmath>

namespace PyImath {

template <class T>
struct rotationXYZWithUpDir_op
{
    static IMATH_NAMESPACE::Vec3<T>
    apply(const IMATH_NAMESPACE::Vec3<T> &from, const IMATH_NAMESPACE::Vec3<T> &to, 
          const IMATH_NAMESPACE::Vec3<T> &up)
    {
        IMATH_NAMESPACE::Vec3<T> retval;
        IMATH_NAMESPACE::extractEulerXYZ(IMATH_NAMESPACE::rotationMatrixWithUpDir(from,to,up),retval);
        return retval;
    }
};

template <class T>
struct abs_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::abs<T>(value);
    }
};

template <class T>
struct sign_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::sign<T>(value);
    }
};

template <class T>
struct log_op
{
    static T
    apply(T value)
    {
        return ::log(value);
    }
};

template <class T>
struct log10_op
{
    static T
    apply(T value)
    {
        return ::log10(value);
    }
};

template <class T>
struct lerp_op
{
    static T
    apply(T a, T b, T t)
    {
        return IMATH_NAMESPACE::lerp<T>(a,b,t);
    }
};

template <class T>
struct ulerp_op
{
    static T
    apply(T a, T b, T t)
    {
        return IMATH_NAMESPACE::ulerp<T>(a,b,t);
    }
};

template <class T>
struct lerpfactor_op
{
    static T
    apply(T a, T b, T t)
    {
        return IMATH_NAMESPACE::lerpfactor<T>(a,b,t);
    }
};

template <class T>
struct clamp_op
{
    static T
    apply(T value, T low, T high)
    {
        return IMATH_NAMESPACE::clamp<T>(value,low,high);
    }
};

template <class T>
struct cmp_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::cmp<T>(value);
    }
};

template <class T>
struct cmpt_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::cmpt<T>(value);
    }
};

template <class T>
struct iszero_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::iszero<T>(value);
    }
};

template <class T>
struct equal_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::equal<T>(value);
    }
};

template <class T>
struct floor_op
{
    static int
    apply(T value)
    {
        return IMATH_NAMESPACE::floor<T>(value);
    }
};

template <class T>
struct ceil_op
{
    static int
    apply(T value)
    {
        return IMATH_NAMESPACE::ceil<T>(value);
    }
};

template <class T>
struct trunc_op
{
    static int
    apply(T value)
    {
        return IMATH_NAMESPACE::trunc<T>(value);
    }
};

struct divs_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::divs(x,y);
    }
};

struct mods_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::mods(x,y);
    }
};

struct divp_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::divp(x,y);
    }
};

struct modp_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::modp(x,y);
    }
};

struct bias_op
{
    static inline float
    apply(float x, float b)
    {
        if (b != 0.5f)
        {
            static const float inverse_log_half = 1.0f / std::log(0.5f);
            const float biasPow = std::log(b)*inverse_log_half;
            return std::pow(x, biasPow);
        }
        return x;
    }
};

struct gain_op
{
    static inline float
    apply(float x, float g)
    {
        if (x < 0.5f)
            return 0.5f*bias_op::apply(2.0f*x, 1.0f - g);
        else
            return 1.0f - 0.5f*bias_op::apply(2.0f - 2.0f*x, 1.0f - g);
    }
};

template <class T>
struct rgb2hsv_op
{
    static inline IMATH_NAMESPACE::Vec3<T>
    apply(const IMATH_NAMESPACE::Vec3<T> &rgb)
    {
        return IMATH_NAMESPACE::rgb2hsv(rgb);
    }
};

template <class T>
struct hsv2rgb_op
{
    static inline IMATH_NAMESPACE::Vec3<T>
    apply(const IMATH_NAMESPACE::Vec3<T> &rgb)
    {
        return IMATH_NAMESPACE::hsv2rgb(rgb);
    }
};

template <class T>
struct min_op
{
    static inline T
    apply(T x, T y)
    {
        return std::min(x,y);
    }
};

template <class T>
struct max_op
{
    static inline T
    apply(T x, T y)
    {
        return std::max(x,y);
    }
};

template <class T>
struct sin_op {
    static inline T
    apply(T theta)
    {
        return std::sin(theta);
    }
};

template <class T>
struct cos_op {
    static inline T
    apply(T theta)
    {
        return std::cos(theta);
    }
};

template <class T>
struct tan_op {
    static inline T
    apply(T theta)
    {
        return std::tan(theta);
    }
};

template <class T>
struct asin_op {
    static inline T
    apply(T x)
    {
        return std::asin(x);
    }
};

template <class T>
struct acos_op {
    static inline T
    apply(T x)
    {
        return std::acos(x);
    }
};

template <class T>
struct atan_op {
    static inline float
    apply(T x)
    {
        return std::atan(x);
    }
};

template <class T>
struct atan2_op {
    static inline T
    apply(T y, T x)
    {
        return std::atan2(y,x);
    }
};

template <class T>
struct sqrt_op {
    static inline T
    apply(T x)
    {
        return std::sqrt(x);
    }
};

template <class T>
struct pow_op {
    static inline T
    apply(T x, T y)
    {
        return std::pow(x,y);
    }
};

template <class T>
struct exp_op
{
    static inline T
    apply(T x)
    {
        return std::exp(x);
    }
};

template <class T>
struct sinh_op
{
    static inline T
    apply(T x)
    {
        return std::sinh(x);
    }
};

template <class T>
struct cosh_op
{
    static inline T
    apply(T x)
    {
        return std::cosh(x);
    }
};

} // namespace PyImath

#endif // _PyImathFunOperators_h_
