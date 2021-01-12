//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef INCLUDED_PYIMATH_DECORATORS_H
#define INCLUDED_PYIMATH_DECORATORS_H

#include <boost/python.hpp>

namespace PyImath
{

// These function add __copy__ and __deepcopy__ methods
// to python classes by simply wrapping the copy constructors
// This interface is needed for using these classes with
// the python copy module.

template <class T>
static T
copy(const T& x)
{
    return T(x);
}

template <class T>
static T
deepcopy(const T& x, boost::python::dict&)
{
    return copy(x);
}

template <class T, class X1, class X2, class X3>
boost::python::class_<T,X1,X2,X3>&
decoratecopy(boost::python::class_<T,X1,X2,X3>& cls)
{
    cls.def("__copy__",&copy<T>);
    cls.def("__deepcopy__",&deepcopy<T>);
    return cls;
}

} // namespace PyImath

#endif // INCLUDED_PYIMATH_DECORATORS_H

