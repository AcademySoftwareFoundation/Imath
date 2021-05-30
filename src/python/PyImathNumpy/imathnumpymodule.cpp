//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <Python.h>
#include <boost/python.hpp>
#include <PyImath.h>
#include <PyImathVec.h>
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

static
object 
arrayToNumpy_float(FloatArray &fa)
{
    if (fa.stride() != 1) {
      throw std::range_error ("Unable to make numpy wrapping of strided arrays");
    }

    npy_intp length = fa.len();
    float *data = &fa[0];
    PyObject *a = PyArray_SimpleNewFromData(1,&length,NPY_FLOAT,data);

    if (!a) {
        throw_error_already_set();
    }
    setBaseObject (a, fa);

    object retval = object (handle<> (a));
    return retval;
}

static
object 
arrayToNumpy_V3f(V3fArray &va)
{
    if (va.stride() != 1) {
      throw std::range_error ("Unable to make numpy wrapping of strided arrays");
    }

    npy_intp length[2];
    length[0] = va.len();
    length[1] = 3;
    float *data = &va[0].x;
    PyObject *a = PyArray_SimpleNewFromData(2,length,NPY_FLOAT,data);

    if (!a) {
        throw_error_already_set();
    }
    setBaseObject (a, va);

    object retval = object(handle<>(a));
    return retval;
}

static
object 
arrayToNumpy_int(IntArray &va)
{
    if (va.stride() != 1) {
      throw std::range_error ("Unable to make numpy wrapping of strided arrays");
    }

    npy_intp length = va.len();
    int *data = &va[0];
    PyObject *a = PyArray_SimpleNewFromData(1,&length,NPY_INT,data);

    if (!a) {
        throw_error_already_set();
    }
    setBaseObject (a, va);

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
BOOST_PYTHON_MODULE(imathnumpy)
{
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

    def("arrayToNumpy",&arrayToNumpy_float,
        "arrayToNumpy(array) - wrap the given FloatArray as a numpy array",
        (arg("array")));
    def("arrayToNumpy",&arrayToNumpy_V3f,
        "arrayToNumpy(array) - wrap the given V3fArray as a numpy array",
        (arg("array")));
    def("arrayToNumpy",&arrayToNumpy_int,
        "arrayToNumpy(array) - wrap the given IntArray as a numpy array",
        (arg("array")));
}
