//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathAPI_h_
#define _PyImathAPI_h_

#include <patchlevel.h>

#if PY_MAJOR_VERSION >= 3

  //  Big changes in Python3 with regard to PyClass.  Most of these functions
  // are gone so the equivalent functionality is done this way...
  #define  PyClass_Check(object) \
           PyObject_IsInstance (object, reinterpret_cast<PyObject *> (&PyType_Type))

  //  Py_FindMethod is gone and so you must search for functions by searching
  // through an object's attributes.
  #define  Py_FindMethod(methods, self, name) \
           PyObject_GenericGetAttr(self, PyBytes_FromString(name))

  //  One of the biggest differences between 2&3 is use support for Unicode.
  // Working with strings at the C API level one has be careful that the
  // returned object will not be Unicode and thus need to be decoded before
  // being interpreted.  These macros will return the PyBytes type of PyObject
  // pointer that replaces PyString.
  #define  PyString_Check(str)                     PyBytes_Check(str)
  #define  PyString_FromString(str)                PyBytes_FromString(str)  
  #define  PyString_AsString(obj)                  PyBytes_AsString(obj)  
  #define  PyString_AsStringAndSize(obj, str, len) PyBytes_AsStringAndSize(obj, str, len)

  //  Python3 interprets all integers as long types and has deprecated PyInt.
  #define  PyInt_Check(x)            PyLong_Check(x)
  #define  PyInt_AsLong(x)           PyLong_AsLong(x)
  #define  PyInt_AS_LONG(x)          PyLong_AsLong(x)
  #define  PyInt_AsSsize_t(x)        PyLong_AsSsize_t(x)
  #define  PyInt_FromLong(x)         PyLong_FromLong(x)

  //  These flags are not present in Python3 and must be replaced with the
  // default set of flags so that OR'ing them together doesn't alter the
  // flags.
  #define  Py_TPFLAGS_CHECKTYPES         Py_TPFLAGS_DEFAULT
  #define  Py_TPFLAGS_HAVE_RICHCOMPARE   Py_TPFLAGS_DEFAULT

  //  The __repr__ for a TypeObject will be encoded and needs to be
  // processed as a PyBytes object before it can be return as a string.
  #define  PYUTIL_OBJECT_REPR(obj)       PyObject_Str (PyObject_Repr (obj))

#else

  //  Python2 code will need to access PyObject_Repr() via this macro so
  // that both 2&3 can compile without modification.
  #define  PYUTIL_OBJECT_REPR(obj)      PyObject_Repr (obj)

#endif

#endif  // _PyImathAPI_h_
