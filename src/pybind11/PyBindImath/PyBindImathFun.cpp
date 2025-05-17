//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"

#include <pybind11/stl.h>
#include <cmath>

#include <ImathColorAlgo.h>
#include <ImathFun.h>
#include <ImathMatrixAlgo.h>
#include <ImathVec.h>

namespace py = pybind11;

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

template <class T>
static inline T
fun_atan2(T y, T x) {
    return std::atan2(y, x);
}

template <class T>
static inline T
fun_pow(T base, T exp) {
    return std::pow(base, exp);
}

void
register_fun(py::module& m)
{
    m.def(
        "abs",
        IMATH_NAMESPACE::abs<int>,
        py::arg("value"),
        "Return the absolute value of the argument.");
    m.def(
        "sign",
        IMATH_NAMESPACE::sign<int>,
        py::arg("value"),
        "Return \"1\" or \"-1\" based on the sign of the argument.");
    m.def(
        "clamp",
        IMATH_NAMESPACE::clamp<int>,
        py::arg("value"),
        py::arg("low"),
        py::arg("high"),
        "Return the first argument clamped using the second and third arguments as a range.");
    m.def(
        "divs",
        IMATH_NAMESPACE::divs,
        py::arg("x"),
        py::arg("y"),
        "Return x/y where the remainder has the same sign as x:\n"
        "    divs(x,y) == (abs(x) / abs(y)) * (sign(x) * sign(y))\n");
    m.def(
        "mods",
        IMATH_NAMESPACE::mods,
        py::arg("x"),
        py::arg("y"),
        "Return x%y where the remainder has the same sign as x:\n"
        "    mods(x,y) == x - y * divs(x,y)\n");
    m.def(
        "divp",
        IMATH_NAMESPACE::divp,
        py::arg("x"),
        py::arg("y"),
        "Return x/y where the remainder is always positive:\n"
        "    divp(x,y) == floor (double(x) / double (y))\n");
    m.def(
        "modp",
        IMATH_NAMESPACE::modp,
        py::arg("x"),
        py::arg("y"),
        "Return x%y where the remainder is always positive:\n"
        "    modp(x,y) == x - y * divp(x,y)\n");
    m.def(
        "succf",
        IMATH_NAMESPACE::succf,
        py::arg("f"));
    m.def(
        "predf",
        IMATH_NAMESPACE::predf,
        py::arg("f"));
    m.def(
        "succd",
        IMATH_NAMESPACE::succd,
        py::arg("d"));
    m.def(
        "predd",
        IMATH_NAMESPACE::predd,
        py::arg("d"));
    m.def(
        "finitef",
        IMATH_NAMESPACE::finitef,
        py::arg("f"));
    m.def(
        "finited",
        IMATH_NAMESPACE::finited,
        py::arg("d"));
    m.def(
        "bias",
        fun_bias,
        py::arg("x"),
        py::arg("b"),
        "Return a gamma correction that remaps the unit interval such that \"bias(0.5, b) = b\".");
    m.def(
        "gain",
        fun_gain,
        py::arg("x"),
        py::arg("g"),
        "Return a gamma correction that remaps the unit interval with the property that \"gain(0.5, g) = 0.5\"."
        "\nThe \"gain()\" function can be thought of as two scaled bias curves forming an \"S\" shape in the unit "
        "interval.");
    m.def(
        "rotationXYZWithUpDir",
        fun_rotationXYZWithUpDir<float>,
        py::arg("fromDir"),
        py::arg("toDir"),
        py::arg("upDir"),
        "Return the XYZ rotation vector that rotates the first vector argument "
        "to the second vector argument, using the third argument as the up-vector.");

    m.def(
        "log",
        [](double value) { return log(value); },
        py::arg("value"),
        "Return the natural logarithm of the argument.");
    m.def(
        "log10",
        [](double value) { return log10(value); },
        py::arg("value"),
        "Return the base 10 logarithm of the argument.");
    m.def(
        "sin",
        [](double theta) { return sin(theta); },
        py::arg("theta"),
        "Return the sine of the argument.");
    m.def(
        "cos",
        [](double theta) { return cos(theta); },
        py::arg("theta"),
        "Return the cosine of the argument.");
    m.def(
        "tan",
        [](double theta) { return std::tan(theta); },
        py::arg("theta"),
        "Return the tangent of the argument.");
    m.def(
        "asin",
        [](double x) { return std::asin(x); },
        py::arg("x"),
        "Return the arcsine of the argument.");
    m.def(
        "acos",
        [](double x) { return std::acos(x); },
        py::arg("x"),
        "Return the arcosine of the argument.");
    m.def(
        "atan",
        [](double x) { return std::atan(x); },
        py::arg("x"),
        "Return the arctangent of the argument.");
    m.def(
        "sqrt",
        [](double x) { return std::sqrt(x); },
        py::arg("x"),
        "Return the square root of the argument.");
    m.def(
        "exp",
        [](double x) { return std::exp(x); },
        py::arg("x"),
        "Return the exponential of the argument.");
    m.def(
        "sinh",
        [](double x) { return std::sinh(x); },
        py::arg("x"),
        "Return the hyperbolic sine of the argument.");
    m.def(
        "cosh",
        [](double x) { return std::cosh(x); },
        py::arg("x"),
        "Return the hyperbolic cosine of the argument.");

}

template <class T>
void
register_fun_fp_T(py::module& m)
{
    m.def(
        "abs",
        IMATH_NAMESPACE::abs<T>,
        py::arg("value"),
        "Return the absolute value of the argument.");
    m.def(
        "sign",
        IMATH_NAMESPACE::sign<T>,
        py::arg("value"),
        "Return \"1\" or \"-1\" based on the sign of the argument.");
    m.def(
        "lerp",
        IMATH_NAMESPACE::lerp<T, T>,
        py::arg("a"),
        py::arg("b"),
        py::arg("t"),
        "Return the linear interpolation of the first and second arguments, "
        "using the third argument as the parameter.");
    m.def(
        "ulerp",
        IMATH_NAMESPACE::ulerp<T, T>,
        py::arg("a"),
        py::arg("b"),
        py::arg("t"));
    m.def(
        "lerpfactor",
        IMATH_NAMESPACE::lerpfactor<T>,
        py::arg("m"),
        py::arg("a"),
        py::arg("b"),
        "Return how far m is between a and b, that is return t such that\n"
        "if:\n"
        "    t = lerpfactor(m, a, b);\n"
        "then:\n"
        "    m = lerp(a, b, t);\n"
        "\n"
        "If a==b, return 0.\n");
    m.def(
        "clamp",
        IMATH_NAMESPACE::clamp<T>,
        py::arg("value"),
        py::arg("low"),
        py::arg("high"),
        "Return the first argument clamped using the second and third arguments as a range.");
    m.def(
        "floor",
        IMATH_NAMESPACE::floor<T>,
        py::arg("value"),
        "Return the closest integer smaller or equal to the argument.");
    m.def(
        "ceil",
        IMATH_NAMESPACE::ceil<T>,
        py::arg("value"),
        "Return the closest integer greater or equal to the argument.");
    m.def(
        "trunc",
        IMATH_NAMESPACE::trunc<T>,
        py::arg("value"),
        "Return the closest integer with a magnitude smaller or equal to the argument.");
    m.def(
        "rgb2hsv",
        fun_rgb2hsv<T>,
        py::arg("rgb"),
        "Return a HSV representation of the RGB argument.");
    m.def(
        "hsv2rgb",
        fun_hsv2rgb<T>,
        py::arg("hsv"),
        "Return a RGB representation of the HSV argument.");
    m.def(
        "cmp",
        IMATH_NAMESPACE::cmp<T>,
        py::arg("a"),
        py::arg("b"));
    m.def(
        "cmpt",
        IMATH_NAMESPACE::cmpt<T>,
        py::arg("a"),
        py::arg("b"),
        py::arg("t"));
    m.def(
        "iszero",
        IMATH_NAMESPACE::iszero<T>,
        py::arg("a"),
        py::arg("t"));
    m.def(
        "equal",
        IMATH_NAMESPACE::equal<T, T, T>,
        py::arg("a"),
        py::arg("b"),
        py::arg("t"));
    m.def(
        "atan2",
        fun_atan2<T>,
        py::arg("y"),
        py::arg("x"),
        "Return the arctangent of the first argument over the second argument.");
    m.def(
        "pow",
        fun_pow<T>,
        py::arg("x"),
        py::arg("y"),
        "Return the first argument raised to the power of the second argument.");
}

} // namespace

namespace PyBindImath {

void
register_imath_fun(py::module& m)
{
    // Bindings for functions using explicit argument(s) and return types.
    register_fun(m);

    // Bindings for functions using floating point types. Only bind
    // to double to prevent loss of precision in inadvertent casting
    // to float.
    // 
    register_fun_fp_T<float>(m);
    register_fun_fp_T<double>(m);
}

} // namespace PyBindImath
