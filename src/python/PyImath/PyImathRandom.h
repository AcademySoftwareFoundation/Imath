//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathRandom_h_
#define _PyImathRandom_h_

#include <Python.h>
#include <boost/python.hpp>
#include <ImathRandom.h>
#include "PyImathExport.h"

namespace PyImath {

PYIMATH_EXPORT boost::python::class_<IMATH_NAMESPACE::Rand32> register_Rand32();
PYIMATH_EXPORT boost::python::class_<IMATH_NAMESPACE::Rand48> register_Rand48();

class PYIMATH_EXPORT Rand32
{
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Rand32 &r);
};

class PYIMATH_EXPORT Rand48
{
  public:
    static PyObject *	wrap (const IMATH_NAMESPACE::Rand48 &r);
};

}

#endif
