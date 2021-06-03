//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <Python.h>
#include <boost/python.hpp>
#include <PyImath.h>
#include <PyImathVec.h>
#include <PyImathColor.h>
#include <iostream>
#include <boost/format.hpp>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

using namespace boost::python;
using namespace PyImath;

template <typename T>
struct Holder
{
    Holder( T &a ) : m_val( a ) {}
    static void Cleanup (PyObject *capsule)
    {
        Holder* h = static_cast<Holder*> (PyCapsule_GetPointer (capsule, NULL));
        delete h;
    }
    T m_val;
};

template <typename T>
static void
setBaseObject (PyObject* nparr, T& arr)
{
    using holder         = Holder<T>;

    holder* ph = new holder (arr);
    PyObject* capsule = PyCapsule_New (ph, NULL, holder::Cleanup);
    PyArray_SetBaseObject ((PyArrayObject*) nparr, capsule);
}

template <typename T> struct NumpyTypeFromType       { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_NOTYPE); };
template <> struct NumpyTypeFromType<signed char>    { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_INT8);   };
template <> struct NumpyTypeFromType<unsigned char>  { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_UINT8);  };
template <> struct NumpyTypeFromType<short>          { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_INT16);  };
template <> struct NumpyTypeFromType<unsigned short> { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_UINT16); };
template <> struct NumpyTypeFromType<int>            { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_INT32);  };
template <> struct NumpyTypeFromType<unsigned int>   { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_UINT32); };
template <> struct NumpyTypeFromType<float>          { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_FLOAT);  };
template <> struct NumpyTypeFromType<double>         { BOOST_STATIC_CONSTANT(int, typeEnum=NPY_DOUBLE); };

template <typename T> struct BaseTypeFrom2DArray       { typedef void type; };
template <> struct BaseTypeFrom2DArray<IntArray2D>     { typedef int type; };
template <> struct BaseTypeFrom2DArray<FloatArray2D>   { typedef float type; };
template <> struct BaseTypeFrom2DArray<DoubleArray2D>  { typedef double type; };
template <> struct BaseTypeFrom2DArray<Color4cArray>   { typedef IMATH_NAMESPACE::Color4c type; };
template <> struct BaseTypeFrom2DArray<Color4fArray>   { typedef IMATH_NAMESPACE::Color4f type; };

template <typename T>
object 
arrayToNumpy_scalar(T &sa)
{
    typedef typename T::BaseType PodType;

    if (sa.stride() != 1)
        throw std::logic_error("Unable to make numpy wrapping of strided arrays");

    int type = NumpyTypeFromType<PodType>::typeEnum;
    npy_intp dims = sa.len();
    PodType *data = &sa[0];
    PyObject *a = PyArray_SimpleNewFromData(1, &dims, type, data);

    if (!a)
        throw_error_already_set();

    setBaseObject (a, sa);

    object retval = object(handle<>(a));
    return retval;
}

template<typename T>
object 
arrayToNumpy_vector(T &va)
{
    typedef typename T::BaseType        BaseType;
    typedef typename BaseType::BaseType PodType;

    if (va.stride() != 1)
        throw std::logic_error("Unable to make numpy wrapping of strided arrays");

    int type = NumpyTypeFromType<PodType>::typeEnum;
    npy_intp dims[2]{ va.len(), BaseType::dimensions()};
    PodType *data = &va[0][0];
    PyObject *a = PyArray_SimpleNewFromData(2, dims, type, data);

    if (!a)
        throw_error_already_set();

    setBaseObject (a, va);

    object retval = object (handle<> (a));
    return retval;
}

template<typename T>
object 
arrayToNumpy_scalar2D(T &ca)
{
    typedef typename BaseTypeFrom2DArray<T>::type PodType;

    // Numpy is matrix indexed based - the first index indicates the row and
    // the second index indicates the column.
    // Zeno data is image indexed based - the first index indicates the column
    // and the second indicates the row.
    // Swap the dimensions so Numpy handles data natively with matrix based
    // indexing.
    IMATH_NAMESPACE::Vec2<size_t> len = ca.len();
    int type = NumpyTypeFromType<PodType>::typeEnum;
    npy_intp dims[2]{ static_cast<npy_intp>(len.y),
                      static_cast<npy_intp>(len.x) };
    PodType *data = &ca(0, 0);
    PyObject *a = PyArray_SimpleNewFromData(2, dims, type, data);

    if (!a)
        throw_error_already_set();

    setBaseObject (a, ca);

    object retval = object(handle<>(a));
    return retval;
}

template<typename T>
object 
arrayToNumpy_vector2D(T &ca)
{
    typedef typename BaseTypeFrom2DArray<T>::type BaseType;
    typedef typename BaseType::BaseType           PodType;

    // Numpy is matrix indexed based - the first index indicates the row and
    // the second index indicates the column.
    // Zeno data is image indexed based - the first index indicates the column
    // and the second indicates the row.
    // Swap the dimensions so Numpy handles data natively with matrix based
    // indexing.
    IMATH_NAMESPACE::Vec2<size_t> len = ca.len();
    int type = NumpyTypeFromType<PodType>::typeEnum;
    npy_intp dims[3]{ static_cast<npy_intp>(len.y),
                      static_cast<npy_intp>(len.x),
                      BaseType::dimensions() };
    PodType *data = &ca(0, 0)[0];
    PyObject *a = PyArray_SimpleNewFromData(3, dims, type, data);

    if (!a)
        throw_error_already_set();

    setBaseObject (a, ca);

    object retval = object(handle<>(a));
    return retval;
}

#if PY_MAJOR_VERSION > 2
static void *apply_import()
{
    import_array();
    return 0;
}
#endif

// Convenience macro for wrapping scalar PyImath array types.
#define WRAP_SCALAR_ARRAY(ARRAY_TYPE)                                            \
    def("arrayToNumpy", &arrayToNumpy_scalar<ARRAY_TYPE>,                        \
        "arrayToNumpy(array) - wrap the given " #ARRAY_TYPE " as a numpy array", \
        (arg("array")));
#define WRAP_SCALAR_ARRAY_2D(ARRAY_TYPE)                                         \
    def("arrayToNumpy", &arrayToNumpy_scalar2D<ARRAY_TYPE>,                      \
        "arrayToNumpy(array) - wrap the given " #ARRAY_TYPE " as a numpy array", \
        (arg("array")));

// Convenience macro for wrapping vector PyImath array types.
#define WRAP_VECTOR_ARRAY(ARRAY_TYPE)                                            \
    def("arrayToNumpy", &arrayToNumpy_vector<ARRAY_TYPE>,                        \
        "arrayToNumpy(array) - wrap the given " #ARRAY_TYPE " as a numpy array", \
        (arg("array")));
#define WRAP_VECTOR_ARRAY_2D(ARRAY_TYPE)                                         \
    def("arrayToNumpy", &arrayToNumpy_vector2D<ARRAY_TYPE>,                      \
        "arrayToNumpy(array) - wrap the given " #ARRAY_TYPE " as a numpy array", \
        (arg("array")));

BOOST_PYTHON_MODULE(imathnumpy)
{
    scope().attr("__doc__") = "Imathnumpy module";
    scope().attr("__version__") = IMATH_VERSION_STRING;

    handle<> imath(PyImport_ImportModule("imath"));
    if (PyErr_Occurred()) throw_error_already_set();
    scope().attr("imath") = imath;

    handle<> numpy(PyImport_ImportModule("numpy"));
    if (PyErr_Occurred()) throw_error_already_set();
    scope().attr("numpy") = numpy;

#if PY_MAJOR_VERSION > 2
    // seems like numpy expects this to be used in a scenario
    // where there is a return value in python3...
    (void)apply_import();
#else
    import_array();
#endif

    scope().attr("__doc__") = "Array wrapping module to overlay imath array data with numpy arrays";

    WRAP_SCALAR_ARRAY(SignedCharArray)
    WRAP_SCALAR_ARRAY(UnsignedCharArray)
    WRAP_SCALAR_ARRAY(ShortArray)
    WRAP_SCALAR_ARRAY(UnsignedShortArray)
    WRAP_SCALAR_ARRAY(IntArray)
    WRAP_SCALAR_ARRAY(UnsignedIntArray)
    WRAP_SCALAR_ARRAY(FloatArray)
    WRAP_SCALAR_ARRAY(DoubleArray)

    WRAP_VECTOR_ARRAY(V2sArray)
    WRAP_VECTOR_ARRAY(V2iArray)
    WRAP_VECTOR_ARRAY(V2fArray)
    WRAP_VECTOR_ARRAY(V2dArray)

    WRAP_VECTOR_ARRAY(V3sArray)
    WRAP_VECTOR_ARRAY(V3iArray)
    WRAP_VECTOR_ARRAY(V3fArray)
    WRAP_VECTOR_ARRAY(V3dArray)

    WRAP_VECTOR_ARRAY(V4sArray)
    WRAP_VECTOR_ARRAY(V4iArray)
    WRAP_VECTOR_ARRAY(V4fArray)
    WRAP_VECTOR_ARRAY(V4dArray)

    WRAP_VECTOR_ARRAY(C3cArray)
    WRAP_VECTOR_ARRAY(C3fArray)
    WRAP_VECTOR_ARRAY(C4cArray)
    WRAP_VECTOR_ARRAY(C4fArray)

    WRAP_SCALAR_ARRAY_2D(IntArray2D)
    WRAP_SCALAR_ARRAY_2D(FloatArray2D)
    WRAP_SCALAR_ARRAY_2D(DoubleArray2D)

    WRAP_VECTOR_ARRAY_2D(Color4cArray)
    WRAP_VECTOR_ARRAY_2D(Color4fArray)
}
