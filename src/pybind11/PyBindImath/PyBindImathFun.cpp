//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"

#include <cmath>

#include <ImathColorAlgo.h>
#include <ImathFun.h>
#include <ImathMatrixAlgo.h>
#include <ImathVec.h>

namespace {

static inline float
fun_bias(float x, float b)
{
    if (b != 0.5f)
    {
        static const float inverse_log_half = 1.0f / std::log(0.5f);
        const float biasPow = std::log(b) * inverse_log_half;

        return std::pow(x, biasPow);
    }

    return x;
}

static inline float
fun_gain(float x, float g)
{
    if (x < 0.5f)
        return 0.5f * fun_bias(2.0f * x, 1.0f - g);
    else
        return 1.0f - 0.5f * fun_bias(2.0f - 2.0f * x, 1.0f - g);
}

template <class T>
static IMATH_NAMESPACE::Vec3<T>
fun_rotationXYZWithUpDir(
    const IMATH_NAMESPACE::Vec3<T> &from,
    const IMATH_NAMESPACE::Vec3<T> &to,
    const IMATH_NAMESPACE::Vec3<T> &up)
{
    IMATH_NAMESPACE::Vec3<T> retval;
    IMATH_NAMESPACE::extractEulerXYZ(IMATH_NAMESPACE::rotationMatrixWithUpDir(from, to, up), retval);

    return retval;
}

template <class T>
static inline IMATH_NAMESPACE::Vec3<T>
fun_rgb2hsv(const IMATH_NAMESPACE::Vec3<T> &rgb)
{
    return IMATH_NAMESPACE::rgb2hsv(rgb);
}

template <class T>
static inline IMATH_NAMESPACE::Vec3<T>
fun_hsv2rgb(const IMATH_NAMESPACE::Vec3<T> &rgb)
{
    return IMATH_NAMESPACE::hsv2rgb(rgb);
}

void
register_fun(pybind11::module& m)
{
    m.def("abs", IMATH_NAMESPACE::abs<int>);
    m.def("sign", IMATH_NAMESPACE::sign<int>);
    m.def("clamp", IMATH_NAMESPACE::clamp<int>);
    m.def("divs", IMATH_NAMESPACE::divs);
    m.def("mods", IMATH_NAMESPACE::mods);
    m.def("divp", IMATH_NAMESPACE::divp);
    m.def("modp", IMATH_NAMESPACE::modp);
    m.def("succf", IMATH_NAMESPACE::succf);
    m.def("predf", IMATH_NAMESPACE::predf);
    m.def("succd", IMATH_NAMESPACE::succd);
    m.def("predd", IMATH_NAMESPACE::predd);
    m.def("finitef", IMATH_NAMESPACE::finitef);
    m.def("finited", IMATH_NAMESPACE::finited);
    m.def("gain", fun_gain);
    m.def("bias", fun_bias);
    m.def("rotationXYZWithUpDir", fun_rotationXYZWithUpDir<float>);
}

template <class T>
void
register_fun_fp_T(pybind11::module& m)
{
    m.def("abs", IMATH_NAMESPACE::abs<T>);
    m.def("sign", IMATH_NAMESPACE::sign<T>);
    m.def("log", ::log<T>);
    m.def("log10", ::log10<T>);
    m.def("lerp", IMATH_NAMESPACE::lerp<T, T>);
    m.def("lerpfactor", IMATH_NAMESPACE::lerpfactor<T>);
    m.def("clamp", IMATH_NAMESPACE::clamp<T>);
    m.def("floor", IMATH_NAMESPACE::floor<T>);
    m.def("ceil", IMATH_NAMESPACE::ceil<T>);
    m.def("trunc", IMATH_NAMESPACE::trunc<T>);
    m.def("rgb2hsv", fun_rgb2hsv<T>);
    m.def("hsv2rgb", fun_hsv2rgb<T>);
    m.def("sin", std::sin<T>);
    m.def("cos", std::cos<T>);
    m.def("tan", std::tan<T>);
    m.def("asin", std::asin<T>);
    m.def("acos", std::acos<T>);
    m.def("atan", std::atan<T>);
    m.def("atan2", std::atan2<T>);
    m.def("sqrt", std::sqrt<T>);
    m.def("pow", std::pow<T>);
    m.def("exp", std::exp<T>);
    m.def("sinh", std::sinh<T>);
    m.def("cosh", std::cosh<T>);
    m.def("cmp", IMATH_NAMESPACE::cmp<T>);
    m.def("cmpt", IMATH_NAMESPACE::cmpt<T>);
    m.def("iszero", IMATH_NAMESPACE::iszero<T>);
    m.def("equal", IMATH_NAMESPACE::equal<T, T, T>);
}

} // namespace

namespace PyBindImath {

void
register_imath_fun(pybind11::module& m)
{
    // Bindings for functions using explicit argument(s) and return types.
    register_fun(m);

    // Bindings for functions using floating point types, i.e. `float` and `double`, for its argument(s)
    // and return types.
    register_fun_fp_T<float>(m);
    register_fun_fp_T<double>(m);
}

} // namespace PyBindImath
