//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <Python.h>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/format.hpp>
#include <ImathVec.h>
#include <ImathQuat.h>
#include <ImathEuler.h>
#include <ImathFun.h>
#include <ImathMatrixAlgo.h>

#include "PyImathFixedArray.h"
#include "PyImath.h"
#include "PyImathExport.h"
#include "PyImathBasicTypes.h"
#include "PyImathVec.h"
#include "PyImathMatrix.h"
#include "PyImathBox.h"
#include "PyImathFun.h"
#include "PyImathQuat.h"
#include "PyImathEuler.h"
#include "PyImathColor.h"
#include "PyImathFrustum.h"
#include "PyImathPlane.h"
#include "PyImathLine.h"
#include "PyImathRandom.h"
#include "PyImathShear.h"
#include "PyImathMathExc.h"
#include "PyImathAutovectorize.h"
#include "PyImathStringArrayRegister.h"
#include "PyImathBufferProtocol.h"

using namespace boost::python;
using namespace PyImath;

namespace {

template <typename T>
IMATH_NAMESPACE::Box<IMATH_NAMESPACE::Vec3<T> >
computeBoundingBox(const FixedArray<IMATH_NAMESPACE::Vec3<T> >& position)
{
    IMATH_NAMESPACE::Box<IMATH_NAMESPACE::Vec3<T> > bounds;
    int len = position.len();
    for (int i = 0; i < len; ++i)
        bounds.extendBy(position[i]);
    return bounds;
}

IMATH_NAMESPACE::M44d
procrustes1 (PyObject* from_input, 
             PyObject* to_input,
             PyObject* weights_input = 0,
             bool doScale = false)
{
    // Verify the sequences:
    if (!PySequence_Check (from_input))
    {
        PyErr_SetString (PyExc_TypeError, "Expected a sequence type for 'from'");
        throw_error_already_set();
    }
        
    if (!PySequence_Check (to_input))
    {
        PyErr_SetString (PyExc_TypeError, "Expected a sequence type for 'to'");
        throw_error_already_set();
    }

    bool useWeights = PySequence_Check (weights_input);

    // Now verify the lengths:
    const Py_ssize_t n = PySequence_Length (from_input);
    if (n != PySequence_Length (to_input) ||
        (useWeights && n != PySequence_Length (weights_input)))
    {
        PyErr_SetString (PyExc_TypeError, "'from, 'to', and 'weights' should all have the same lengths.");
        throw_error_already_set();
    }

    std::vector<IMATH_NAMESPACE::V3d> from;  from.reserve (n);
    std::vector<IMATH_NAMESPACE::V3d> to;    to.reserve (n);
    std::vector<double> weights;   weights.reserve (n);

    for (Py_ssize_t i = 0; i < n; ++i)
    {
        PyObject* f = PySequence_GetItem (from_input, i);
        PyObject* t = PySequence_GetItem (to_input, i);
        PyObject* w = 0;
        if (useWeights)
            w = PySequence_GetItem (weights_input, i);

        if (f == 0 || t == 0 || (useWeights && w == 0))
        {
            PyErr_SetString (PyExc_TypeError,
                             "Missing element in array");
            throw_error_already_set();
        }

        from.push_back (extract<IMATH_NAMESPACE::V3d> (f));
        to.push_back (extract<IMATH_NAMESPACE::V3d> (t));
        if (useWeights)
            weights.push_back (extract<double> (w));
    }

    if (useWeights)
        return IMATH_NAMESPACE::procrustesRotationAndTranslation (&from[0], &to[0], &weights[0], n, doScale);
    else
        return IMATH_NAMESPACE::procrustesRotationAndTranslation (&from[0], &to[0], n, doScale);
}

template <typename T>
const T*
flatten(const PyImath::FixedArray<T>& q, std::unique_ptr<T[]>& handle)
{
    if (q.isMaskedReference())
    {
        const size_t len = q.len();
        handle.reset(new T[len]);
        for (size_t i = 0; i < len; ++i)
            handle[i] = q[i];

        return handle.get();
    }

    return &q[0];
}

template <typename T>
IMATH_NAMESPACE::M44d
procrustesRotationAndTranslation(const FixedArray<IMATH_NAMESPACE::Vec3<T> >& from,
                                 const FixedArray<IMATH_NAMESPACE::Vec3<T> >& to,
                                 const FixedArray<T>* weights = 0,
                                 bool doScale = false)
{
    const size_t len = from.match_dimension(to);
    if (len == 0)
        return IMATH_NAMESPACE::M44d();

    std::unique_ptr<IMATH_NAMESPACE::Vec3<T>[]> fromHandle;
    const IMATH_NAMESPACE::Vec3<T>* fromPtr = flatten(from, fromHandle);

    std::unique_ptr<IMATH_NAMESPACE::Vec3<T>[]> toHandle;
    const IMATH_NAMESPACE::Vec3<T>* toPtr = flatten(to, toHandle);

    std::unique_ptr<T[]> weightsHandle;
    const T* weightsPtr = nullptr;
    if (weights)
    {
        weights->match_dimension(from);
        flatten(*weights, weightsHandle);
    }

    if (weightsPtr)
        return IMATH_NAMESPACE::procrustesRotationAndTranslation(fromPtr, toPtr, weightsPtr, len, doScale);
    else
        return IMATH_NAMESPACE::procrustesRotationAndTranslation(fromPtr, toPtr, len, doScale);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(procrustesRotationAndTranslationf_overloads, procrustesRotationAndTranslation<float>, 2, 4);
BOOST_PYTHON_FUNCTION_OVERLOADS(procrustesRotationAndTranslationd_overloads, procrustesRotationAndTranslation<double>, 2, 4);


FixedArray2D<int> rangeX(int sizeX, int sizeY)
{
    FixedArray2D<int> f(sizeX, sizeY);
    for (int j=0; j<sizeY; j++)
        for (int i=0; i<sizeX; i++)
            f(i,j) = i;
    return f;
}

FixedArray2D<int> rangeY(int sizeX, int sizeY)
{
    FixedArray2D<int> f(sizeX, sizeY);
    for (int j=0; j<sizeY; j++)
        for (int i=0; i<sizeX; i++)
            f(i,j) = j;
    return f;
}

} // anonymous-namespace


BOOST_PYTHON_MODULE(imath)
{
    scope().attr("__doc__") = "Imath module";
    scope().attr("__version__") = IMATH_VERSION_STRING;

    register_basicTypes();

    class_<IntArray2D> iclass2D = IntArray2D::register_("IntArray2D","Fixed length array of ints");
    add_arithmetic_math_functions(iclass2D);
    add_mod_math_functions(iclass2D);
    add_comparison_functions(iclass2D);
    add_ordered_comparison_functions(iclass2D);
    add_explicit_construction_from_type<float>(iclass2D);
    add_explicit_construction_from_type<double>(iclass2D);

    class_<IntMatrix> imclass = IntMatrix::register_("IntMatrix","Fixed size matrix of ints");
    add_arithmetic_math_functions(imclass);

    class_<FloatArray2D> fclass2D = FloatArray2D::register_("FloatArray2D","Fixed length 2D array of floats");
    add_arithmetic_math_functions(fclass2D);
    add_pow_math_functions(fclass2D);
    add_comparison_functions(fclass2D);
    add_ordered_comparison_functions(fclass2D);
    add_explicit_construction_from_type<int>(fclass2D);
    add_explicit_construction_from_type<double>(fclass2D);

    class_<FloatMatrix> fmclass = FloatMatrix::register_("FloatMatrix","Fixed size matrix of floats");
    add_arithmetic_math_functions(fmclass);
    add_pow_math_functions(fmclass);

    class_<DoubleArray2D> dclass2D = DoubleArray2D::register_("DoubleArray2D","Fixed length array of doubles");
    add_arithmetic_math_functions(dclass2D);
    add_pow_math_functions(dclass2D);
    add_comparison_functions(dclass2D);
    add_ordered_comparison_functions(dclass2D);
    add_explicit_construction_from_type<int>(dclass2D);
    add_explicit_construction_from_type<float>(dclass2D);

    class_<DoubleMatrix> dmclass = DoubleMatrix::register_("DoubleMatrix","Fixed size matrix of doubles");
    add_arithmetic_math_functions(dmclass);
    add_pow_math_functions(dmclass);

    def("rangeX", &rangeX);
    def("rangeY", &rangeY);

    def("IntArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<int> >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct an IntArray from a buffer object");

    def("FloatArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<float> >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a FloatArray from a buffer object");

    def("DoubleArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<double> >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a DoubleArray from a buffer object");

    //
    //  Vec2
    //
    register_Vec2<short>();
    register_Vec2<int>();
    register_Vec2<int64_t>();
    register_Vec2<float>();
    register_Vec2<double>();
    class_<FixedArray<IMATH_NAMESPACE::V2s> >   v2s_class = register_Vec2Array<short>();
    class_<FixedArray<IMATH_NAMESPACE::V2i> >   v2i_class = register_Vec2Array<int>();
    class_<FixedArray<IMATH_NAMESPACE::V2i64> > v2i64_class = register_Vec2Array<int64_t>();
    class_<FixedArray<IMATH_NAMESPACE::V2f> >   v2f_class = register_Vec2Array<float>();
    class_<FixedArray<IMATH_NAMESPACE::V2d> >   v2d_class = register_Vec2Array<double>();

    add_explicit_construction_from_type<IMATH_NAMESPACE::V2s>(v2i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2s>(v2i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2s>(v2f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2s>(v2d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i>(v2s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i>(v2i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i>(v2f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i>(v2d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i64>(v2s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i64>(v2i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i64>(v2f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i64>(v2d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V2f>(v2s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2f>(v2i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2f>(v2i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2f>(v2d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V2d>(v2s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2d>(v2i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2d>(v2i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2d>(v2f_class);

    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V2s> > (v2s_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V2i> > (v2i_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V2i64> > (v2i64_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V2f> > (v2f_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V2d> > (v2d_class);

    def("V2iArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec2<int> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V2iArray from a buffer object");

    def("V2fArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec2<float> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V2fArray from a buffer object");

    def("V2dArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec2<double> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V2dArray from a buffer object");

    //
    //  Vec3
    //
    register_Vec3<unsigned char>();
    register_Vec3<short>();
    register_Vec3<int>();
    register_Vec3<int64_t>();
    register_Vec3<float>();
    register_Vec3<double>();
    class_<FixedArray<IMATH_NAMESPACE::V3s> >   v3s_class = register_Vec3Array<short>();
    class_<FixedArray<IMATH_NAMESPACE::V3i> >   v3i_class = register_Vec3Array<int>();
    class_<FixedArray<IMATH_NAMESPACE::V3i64> > v3i64_class = register_Vec3Array<int64_t>();
    class_<FixedArray<IMATH_NAMESPACE::V3f> >   v3f_class = register_Vec3Array<float>();
    class_<FixedArray<IMATH_NAMESPACE::V3d> >   v3d_class = register_Vec3Array<double>();

    add_explicit_construction_from_type<IMATH_NAMESPACE::V3s>(v3i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3s>(v3i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3s>(v3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3s>(v3d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i>(v3s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i>(v3i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i>(v3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i>(v3d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i64>(v3s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i64>(v3i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i64>(v3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i64>(v3d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(v3s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(v3i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(v3i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(v3d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(v3s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(v3i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(v3i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(v3f_class);

    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V3s> > (v3s_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V3i> > (v3i_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V3i64> > (v3i64_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V3f> > (v3f_class);
    add_buffer_protocol<FixedArray<IMATH_NAMESPACE::V3d> > (v3d_class);

    def("V3iArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec3<int> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V3iArray from a buffer object");

    def("V3fArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec3<float> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V3fArray from a buffer object");

    def("V3dArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec3<double> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V3dArray from a buffer object");

    //
    //  Vec4
    //
    register_Vec4<unsigned char>();
    register_Vec4<short>();
    register_Vec4<int>();
    register_Vec4<int64_t>();
    register_Vec4<float>();
    register_Vec4<double>();
    class_<FixedArray<IMATH_NAMESPACE::V4s> >   v4s_class = register_Vec4Array<short>();
    class_<FixedArray<IMATH_NAMESPACE::V4i> >   v4i_class = register_Vec4Array<int>();
    class_<FixedArray<IMATH_NAMESPACE::V4i64> > v4i64_class = register_Vec4Array<int64_t>();
    class_<FixedArray<IMATH_NAMESPACE::V4f> >   v4f_class = register_Vec4Array<float>();
    class_<FixedArray<IMATH_NAMESPACE::V4d> >   v4d_class = register_Vec4Array<double>();

    add_explicit_construction_from_type<IMATH_NAMESPACE::V4s>(v4i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4s>(v4i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4s>(v4f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4s>(v4d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i>(v4s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i>(v4i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i>(v4f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i>(v4d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i64>(v4s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i64>(v4i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i64>(v4f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i64>(v4d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V4f>(v4s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4f>(v4i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4f>(v4i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4f>(v4d_class);

    add_explicit_construction_from_type<IMATH_NAMESPACE::V4d>(v4s_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4d>(v4i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4d>(v4i64_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4d>(v4f_class);

    def("V4iArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec4<int> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V4iArray from a buffer object");

    def("V4fArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec4<float> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V4fArray from a buffer object");

    def("V4dArrayFromBuffer", &fixedArrayFromBuffer<FixedArray<IMATH_NAMESPACE::Vec4<double> > >,
        return_value_policy<manage_new_object>(),
        args("bufferObject"),
        "Construct a V4dArray from a buffer object");

    //
    //  Quat
    //
    register_Quat<float>();
    register_Quat<double>();
    class_<FixedArray<IMATH_NAMESPACE::Quatf> > quatf_class = register_QuatArray<float>();
    class_<FixedArray<IMATH_NAMESPACE::Quatd> > quatd_class = register_QuatArray<double>();
    add_explicit_construction_from_type<IMATH_NAMESPACE::Quatd>(quatf_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::Quatf>(quatd_class);

    //
    // Euler
    //
    register_Euler<float>();
    register_Euler<double>();
    class_<FixedArray<IMATH_NAMESPACE::Eulerf> > eulerf_class = register_EulerArray<float>();
    class_<FixedArray<IMATH_NAMESPACE::Eulerd> > eulerd_class = register_EulerArray<double>();
    add_explicit_construction_from_type<IMATH_NAMESPACE::Eulerd>(eulerf_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::Eulerf>(eulerd_class);

    //
    // Box2
    //
    register_Box2<IMATH_NAMESPACE::V2s>();
    register_Box2<IMATH_NAMESPACE::V2i>();
    register_Box2<IMATH_NAMESPACE::V2i64>();
    register_Box2<IMATH_NAMESPACE::V2f>();
    register_Box2<IMATH_NAMESPACE::V2d>();
    class_<FixedArray<IMATH_NAMESPACE::Box2s> >   b2s_class =   register_BoxArray<IMATH_NAMESPACE::V2s>();
    class_<FixedArray<IMATH_NAMESPACE::Box2i> >   b2i_class =   register_BoxArray<IMATH_NAMESPACE::V2i>();
    class_<FixedArray<IMATH_NAMESPACE::Box2i64> > b2i64_class = register_BoxArray<IMATH_NAMESPACE::V2i64>();
    class_<FixedArray<IMATH_NAMESPACE::Box2f> >   b2f_class =   register_BoxArray<IMATH_NAMESPACE::V2f>();
    class_<FixedArray<IMATH_NAMESPACE::Box2d> >   b2d_class =   register_BoxArray<IMATH_NAMESPACE::V2d>();

    //
    // Box3
    //
    register_Box3<IMATH_NAMESPACE::V3s>();
    register_Box3<IMATH_NAMESPACE::V3i>();
    register_Box3<IMATH_NAMESPACE::V3i64>();
    register_Box3<IMATH_NAMESPACE::V3f>();
    register_Box3<IMATH_NAMESPACE::V3d>();
    class_<FixedArray<IMATH_NAMESPACE::Box3s> >   b3s_class =   register_BoxArray<IMATH_NAMESPACE::V3s>();
    class_<FixedArray<IMATH_NAMESPACE::Box3i> >   b3i_class =   register_BoxArray<IMATH_NAMESPACE::V3i>();
    class_<FixedArray<IMATH_NAMESPACE::Box3i64> > b3i64_class = register_BoxArray<IMATH_NAMESPACE::V3i64>();
    class_<FixedArray<IMATH_NAMESPACE::Box3f> >   b3f_class =   register_BoxArray<IMATH_NAMESPACE::V3f>();
    class_<FixedArray<IMATH_NAMESPACE::Box3d> >   b3d_class =   register_BoxArray<IMATH_NAMESPACE::V3d>();

    //
    // Matrix22/33/44
    //
    register_Matrix22<float>();
    register_Matrix22<double>();
    register_Matrix33<float>();
    register_Matrix33<double>();
    register_Matrix44<float>();
    register_Matrix44<double>();

    //
    // M22/M33/44Array
    //
    class_<FixedArray<IMATH_NAMESPACE::M44d> > m44d_class = register_M44Array<double>();
    class_<FixedArray<IMATH_NAMESPACE::M44f> > m44f_class = register_M44Array<float>();
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix44<double> >(m44d_class);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix44<float> > (m44f_class);

    class_<FixedArray<IMATH_NAMESPACE::M33d> > m33d_class = register_M33Array<double>();
    class_<FixedArray<IMATH_NAMESPACE::M33f> > m33f_class = register_M33Array<float>();
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix33<double> >(m33d_class);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix33<float> > (m33f_class);

    class_<FixedArray<IMATH_NAMESPACE::M22d> > m22d_class = register_M22Array<double>();
    class_<FixedArray<IMATH_NAMESPACE::M22f> > m22f_class = register_M22Array<float>();
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix22<double> >(m22d_class);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix22<float> > (m22f_class);

    //
    // String Array
    //
    register_StringArrays();

    //
    // Color3/4
    //
    register_Color3<unsigned char>();
    register_Color3<float>();
    register_Color4<unsigned char>();
    register_Color4<float>();

    //
    // C3/4Array
    //
    class_<FixedArray<IMATH_NAMESPACE::Color3f> > c3f_class = register_Color3Array<float>();
    class_<FixedArray<IMATH_NAMESPACE::Color3c> > c3c_class = register_Color3Array<unsigned char>();
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(c3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(c3f_class);

    class_<FixedArray<IMATH_NAMESPACE::Color4f> > c4f_class = register_Color4Array<float>();
    class_<FixedArray<IMATH_NAMESPACE::Color4c> > c4c_class = register_Color4Array<unsigned char>();

    //
    // Color4Array
    //
    register_Color4Array2D<float>();
    register_Color4Array2D<unsigned char>();

    //
    // Frustum
    //
    register_Frustum<float>();
    register_Frustum<double>();
    register_FrustumTest<float>();
    register_FrustumTest<double>();

    //
    // Plane
    //
    register_Plane<float>();
    register_Plane<double>();

    //
    // Line
    //
    register_Line<float>();
    register_Line<double>();

    //
    // Shear
    //
    register_Shear<float>();
    register_Shear<double>();

    //
    // Utility Functions
    //
    register_functions();
   

    def("procrustesRotationAndTranslation", procrustes1, 
        args("fromPts", "toPts", "weights", "doScale"),  // Can't use 'from' and 'to' because 'from' is a reserved keywork in Python
        "Computes the orthogonal transform (consisting only of rotation and translation) mapping the "
        "'fromPts' points as close as possible to the 'toPts' points in the least squares norm.  The 'fromPts' and "
        "'toPts' lists must be the same length or the function will error out.  If weights "
        "are provided, then the points are weighted (that is, some points are considered more important "
        "than others while computing the transform).  If the 'doScale' parameter is True, then "
        "the resulting matrix is also allowed to have a uniform scale.");

    def("procrustesRotationAndTranslation", &procrustesRotationAndTranslation<float>, procrustesRotationAndTranslationf_overloads(
        args("fromPts", "toPts", "weights", "doScale"),
        "Computes the orthogonal transform (consisting only of rotation and translation) mapping the "
        "'fromPts' points as close as possible to the 'toPts' points in the least squares norm.  The 'fromPts' and "
        "'toPts' lists must be the same length or the function will error out.  If weights "
        "are provided, then the points are weighted (that is, some points are considered more important "
        "than others while computing the transform).  If the 'doScale' parameter is True, then "
        "the resulting matrix is also allowed to have a uniform scale."));

    def("procrustesRotationAndTranslation", &procrustesRotationAndTranslation<double>, procrustesRotationAndTranslationd_overloads(
        args("fromPts", "toPts", "weights", "doScale"),
        "Computes the orthogonal transform (consisting only of rotation and translation) mapping the "
        "'fromPts' points as close as possible to the 'toPts' points in the least squares norm.  The 'fromPts' and "
        "'toPts' lists must be the same length or the function will error out.  If weights "
        "are provided, then the points are weighted (that is, some points are considered more important "
        "than others while computing the transform).  If the 'doScale' parameter is True, then "
        "the resulting matrix is also allowed to have a uniform scale."));

    //
    // Rand
    //
    register_Rand32();
    register_Rand48();
    
    //
    // Initialize constants
    //

    scope().attr("EULER_XYZ")    = IMATH_NAMESPACE::Eulerf::XYZ;
    scope().attr("EULER_XZY")    = IMATH_NAMESPACE::Eulerf::XZY;
    scope().attr("EULER_YZX")    = IMATH_NAMESPACE::Eulerf::YZX;
    scope().attr("EULER_YXZ")    = IMATH_NAMESPACE::Eulerf::YXZ;
    scope().attr("EULER_ZXY")    = IMATH_NAMESPACE::Eulerf::ZXY;
    scope().attr("EULER_ZYX")    = IMATH_NAMESPACE::Eulerf::ZYX;
    scope().attr("EULER_XZX")    = IMATH_NAMESPACE::Eulerf::XZX;
    scope().attr("EULER_XYX")    = IMATH_NAMESPACE::Eulerf::XYX;
    scope().attr("EULER_YXY")    = IMATH_NAMESPACE::Eulerf::YXY;
    scope().attr("EULER_YZY")    = IMATH_NAMESPACE::Eulerf::YZY;
    scope().attr("EULER_ZYZ")    = IMATH_NAMESPACE::Eulerf::ZYZ;
    scope().attr("EULER_ZXZ")    = IMATH_NAMESPACE::Eulerf::ZXZ;
    scope().attr("EULER_XYZr")   = IMATH_NAMESPACE::Eulerf::XYZr;
    scope().attr("EULER_XZYr")   = IMATH_NAMESPACE::Eulerf::XZYr;
    scope().attr("EULER_YZXr")   = IMATH_NAMESPACE::Eulerf::YZXr;
    scope().attr("EULER_YXZr")   = IMATH_NAMESPACE::Eulerf::YXZr;
    scope().attr("EULER_ZXYr")   = IMATH_NAMESPACE::Eulerf::ZXYr;
    scope().attr("EULER_ZYXr")   = IMATH_NAMESPACE::Eulerf::ZYXr;
    scope().attr("EULER_XZXr")   = IMATH_NAMESPACE::Eulerf::XZXr;
    scope().attr("EULER_XYXr")   = IMATH_NAMESPACE::Eulerf::XYXr;
    scope().attr("EULER_YXYr")   = IMATH_NAMESPACE::Eulerf::YXYr;
    scope().attr("EULER_YZYr")   = IMATH_NAMESPACE::Eulerf::YZYr;
    scope().attr("EULER_ZYZr")   = IMATH_NAMESPACE::Eulerf::ZYZr;
    scope().attr("EULER_ZXZr")   = IMATH_NAMESPACE::Eulerf::ZXZr;
    scope().attr("EULER_X_AXIS") = IMATH_NAMESPACE::Eulerf::X;
    scope().attr("EULER_Y_AXIS") = IMATH_NAMESPACE::Eulerf::Y;
    scope().attr("EULER_Z_AXIS") = IMATH_NAMESPACE::Eulerf::Z;
    scope().attr("EULER_IJKLayout") = IMATH_NAMESPACE::Eulerf::IJKLayout;
    scope().attr("EULER_XYZLayout") = IMATH_NAMESPACE::Eulerf::XYZLayout;
    
    scope().attr("INT_MIN")      = std::numeric_limits<int>::min();
    scope().attr("INT_MAX")      = std::numeric_limits<int>::max();
    scope().attr("INT_LOWEST")   = std::numeric_limits<int>::lowest();
    scope().attr("INT_EPS")      = std::numeric_limits<int>::epsilon();

    scope().attr("FLT_MIN")      = std::numeric_limits<float>::min();
    scope().attr("FLT_MAX")      = std::numeric_limits<float>::max();
    scope().attr("FLT_LOWEST")   = std::numeric_limits<float>::lowest();
    scope().attr("FLT_EPS")      = std::numeric_limits<float>::epsilon();

    scope().attr("DBL_MIN")      = std::numeric_limits<double>::min();
    scope().attr("DBL_MAX")      = std::numeric_limits<double>::max();
    scope().attr("DBL_LOWEST")   = std::numeric_limits<double>::lowest();
    scope().attr("DBL_EPS")      = std::numeric_limits<double>::epsilon();

    def("computeBoundingBox", &computeBoundingBox<float>,
        "computeBoundingBox(position) -- computes the bounding box from the position array.");

    def("computeBoundingBox", &computeBoundingBox<double>,
        "computeBoundingBox(position) -- computes the bounding box from the position array.");
}

