//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/format.hpp>
#include "PyImathFun.h"
#include "PyImathFunOperators.h"
#include "PyImathDecorators.h"
#include "PyImathExport.h"
#include "PyImathAutovectorize.h"

namespace PyImath {

using namespace boost::python;

namespace
{

struct RegisterFloatDoubleOps
{
    template <typename T>
    void operator() (T)
    {
        // nb: MSVC gets confused about which arg we want (it thinks it
        // might be boost::arg), so telling it which one explicitly here.
        typedef boost::python::arg arg;

        generate_bindings<abs_op<T>,boost::mpl::true_>(
            "abs",
            "return the absolute value of 'value'",
            (arg("value")));

        generate_bindings<sign_op<T>,boost::mpl::true_>(
            "sign",
            "return 1 or -1 based on the sign of 'value'",
            (arg("value")));

        generate_bindings<log_op<T>,boost::mpl::true_>(
            "log",
            "return the natural log of 'value'",
            (arg("value")));

        generate_bindings<log10_op<T>,boost::mpl::true_>(
            "log10",
            "return the base 10 log of 'value'",
            (arg("value")));

        generate_bindings<lerp_op<T>,boost::mpl::true_,boost::mpl::true_,boost::mpl::true_>(
            "lerp",
            "return the linear interpolation of 'a' to 'b' using parameter 't'",
            (arg("a"),arg("b"),arg("t")));

        generate_bindings<lerpfactor_op<T>,boost::mpl::true_,boost::mpl::true_,boost::mpl::true_>(
            "lerpfactor",
            "return how far m is between a and b, that is return t such that\n"
            "if:\n"
            "    t = lerpfactor(m, a, b);\n"
            "then:\n"
            "    m = lerp(a, b, t);\n"
            "\n"
            "If a==b, return 0.\n",
            (arg("m"),arg("a"),arg("b")));

        generate_bindings<clamp_op<T>,boost::mpl::true_,boost::mpl::true_,boost::mpl::true_>(
            "clamp",
            "return the value clamped to the range [low,high]",
            (arg("value"),arg("low"),arg("high")));

        generate_bindings<floor_op<T>,boost::mpl::true_>(
            "floor",
            "return the closest integer less than or equal to 'value'",
            (arg("value")));

        generate_bindings<ceil_op<T>,boost::mpl::true_>(
            "ceil",
            "return the closest integer greater than or equal to 'value'",
            (arg("value")));

        generate_bindings<trunc_op<T>,boost::mpl::true_>(
            "trunc",
            "return the closest integer with magnitude less than or equal to 'value'",
            (arg("value")));

        generate_bindings<rgb2hsv_op<T>,boost::mpl::true_>(
            "rgb2hsv",
            "return the hsv version of an rgb color",
            args("rgb"));

        generate_bindings<hsv2rgb_op<T>,boost::mpl::true_>(
            "hsv2rgb",
            "return the rgb version of an hsv color",
            args("hsv"));

        generate_bindings<sin_op<T>,boost::mpl::true_>(
            "sin",
            "return the sine of the angle theta",
            args("theta"));

        generate_bindings<cos_op<T>,boost::mpl::true_>(
            "cos",
            "return the cosine of the angle theta",
            args("theta"));

        generate_bindings<tan_op<T>,boost::mpl::true_>(
            "tan",
            "return the tangent of the angle theta",
            args("theta"));

        generate_bindings<asin_op<T>,boost::mpl::true_>(
            "asin",
            "return the arcsine of the value x",
            args("x"));

        generate_bindings<acos_op<T>,boost::mpl::true_>(
            "acos",
            "return the arccosine of the value x",
            args("x"));

        generate_bindings<atan_op<T>,boost::mpl::true_>(
            "atan",
            "return the arctangent of the value x",
            args("x"));

        generate_bindings<atan2_op<T>,boost::mpl::true_,boost::mpl::true_>(
            "atan2",
            "return the arctangent of the coordinate x,y - note the y "
            "is the first argument for consistency with libm ordering",
            args("y","x"));

        generate_bindings<sqrt_op<T>,boost::mpl::true_>(
            "sqrt",
            "return the square root of x",
            args("x"));

        generate_bindings<pow_op<T>,boost::mpl::true_,boost::mpl::true_>(
            "pow",
            "return x**y",
            args("x","y"));

        generate_bindings<exp_op<T>,boost::mpl::true_>(
             "exp",
             "return exp(x)",
             args("x"));

        generate_bindings<sinh_op<T>,boost::mpl::true_>(
             "sinh",
             "return sinh(x)",
             args("x"));

        generate_bindings<cosh_op<T>,boost::mpl::true_>(
             "cosh",
             "return cosh(x)",
             args("x"));

        def("cmp", IMATH_NAMESPACE::cmp<T>);
        def("cmpt", IMATH_NAMESPACE::cmpt<T>);
        def("iszero", IMATH_NAMESPACE::iszero<T>);
        def("equal", IMATH_NAMESPACE::equal<T, T, T>);
    }
};

} // namespace

void register_functions()
{
    //
    // Utility Functions
    //

    // nb: MSVC gets confused about which arg we want (it thinks it
    // might be boost::arg), so telling it which one explicitly here.
    typedef boost::python::arg arg;

    using fp_types = boost::mpl::vector<float, double>;
    boost::mpl::for_each<fp_types>(RegisterFloatDoubleOps());

    generate_bindings<abs_op<int>,boost::mpl::true_>(
        "abs",
        "return the absolute value of 'value'",
        (arg("value")));
    
    generate_bindings<sign_op<int>,boost::mpl::true_>(
        "sign",
        "return 1 or -1 based on the sign of 'value'",
        (arg("value")));
    
    generate_bindings<clamp_op<int>,boost::mpl::true_,boost::mpl::true_,boost::mpl::true_>(
        "clamp",
        "return the value clamped to the range [low,high]",
        (arg("value"),arg("low"),arg("high")));

    generate_bindings<divs_op,boost::mpl::true_,boost::mpl::true_>(
        "divs",
        "return x/y where the remainder has the same sign as x:\n"
        "    divs(x,y) == (abs(x) / abs(y)) * (sign(x) * sign(y))\n",
        (arg("x"),arg("y")));
    generate_bindings<mods_op,boost::mpl::true_,boost::mpl::true_>(
        "mods",
        "return x%y where the remainder has the same sign as x:\n"
        "    mods(x,y) == x - y * divs(x,y)\n",
        (arg("x"),arg("y")));

    generate_bindings<divp_op,boost::mpl::true_,boost::mpl::true_>(
        "divp",
        "return x/y where the remainder is always positive:\n"
        "    divp(x,y) == floor (double(x) / double (y))\n",
        (arg("x"),arg("y")));
    generate_bindings<modp_op,boost::mpl::true_,boost::mpl::true_>(
        "modp",
        "return x%y where the remainder is always positive:\n"
        "    modp(x,y) == x - y * divp(x,y)\n",
        (arg("x"),arg("y")));

    generate_bindings<bias_op,boost::mpl::true_,boost::mpl::true_>(
         "bias",
         "bias(x,b) is a gamma correction that remaps the unit interval such that bias(0.5, b) = b.",
         (arg("x"),arg("b")));

    generate_bindings<gain_op,boost::mpl::true_,boost::mpl::true_>(
         "gain",
         "gain(x,g) is a gamma correction that remaps the unit interval with the property that gain(0.5, g) = 0.5.\n"
         "The gain function can be thought of as two scaled bias curves forming an 'S' shape in the unit interval.",
         (arg("x"),arg("g")));

    //
    // Vectorized utility functions
    // 
    generate_bindings<rotationXYZWithUpDir_op<float>,boost::mpl::true_,boost::mpl::true_,boost::mpl::true_>(
        "rotationXYZWithUpDir",
        "return the XYZ rotation vector that rotates 'fromDir' to 'toDir'"
        "using the up vector 'upDir'",
        args("fromDir","toDir","upDir"));
}

} // namespace PyImath
