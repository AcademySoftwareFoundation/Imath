//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef INCLUDED_PYIMATHUTIL_H
#define INCLUDED_PYIMATHUTIL_H

//----------------------------------------------------------------------------
//
//	PyImathUtil.h -- miscellaneous classes, functions
//	and macros that are useful for Python wrapping
//	of C++ objects.
//
//----------------------------------------------------------------------------

#include <Python.h>
#include "PyImathExport.h"
#include <PyImathAPI.h>

namespace PyImath {


/**
 * PyAcquireLock ensures that python is prepared for multi-threaded use and
 * ensures that this thread has the global lock.
 *
 * This object must be instantiated (and continue to be in scope) during all
 * threaded api calls.  It assumes the python interpretter is instantiated and
 * multithreading is enabled.
 * 
 * Note: this is not compatible with additional interpreters (calls to
 * Py_NewInterpreter()); 
 */
class PyAcquireLock
{
  public:
    PYIMATH_EXPORT PyAcquireLock();
    PYIMATH_EXPORT ~PyAcquireLock();

    PYIMATH_EXPORT PyAcquireLock(const PyAcquireLock& other) = delete;
    PYIMATH_EXPORT PyAcquireLock & operator = (PyAcquireLock& other) = delete;
    PYIMATH_EXPORT PyAcquireLock(PyAcquireLock&& other) = delete;
    PYIMATH_EXPORT PyAcquireLock & operator = (PyAcquireLock&& other) = delete;
    
  private:
    PyGILState_STATE _gstate;
};


/**
 * This object causes the python global lock to be released for the duration
 * of it's existence.
 *
 * This object should be instantiated (and continue to be in scope) in thread-
 * safe c++ functions called from python.  This call is designed to be
 * instantiated while an AcquireLock is in effect (nested).
 *
 */
class PyReleaseLock
{
  public:
    PYIMATH_EXPORT PyReleaseLock();
    PYIMATH_EXPORT ~PyReleaseLock();
    PYIMATH_EXPORT PyReleaseLock(const PyReleaseLock& other) = delete;
    PYIMATH_EXPORT PyReleaseLock & operator = (PyReleaseLock& other) = delete;
    PYIMATH_EXPORT PyReleaseLock(PyReleaseLock&& other) = delete;
    PYIMATH_EXPORT PyReleaseLock & operator = (PyReleaseLock&& other) = delete;

  private:
    PyThreadState *_save;

};

/**
 * This object is safe object wrapper intended to use with boost python objects.
 *
 * This object correctly acquires the python lock for creation, copying and
 * desctruction of the given object.
 *
 */
template <class T>
class PySafeObject
{
  public:
    PySafeObject()
        : _object(0)
    {
        PyAcquireLock pylock;
        _object = new T();
    }

    PySafeObject(const T &value)
        : _object(0)
    {
        PyAcquireLock pylock;
        _object = new T(value);
    }

    ~PySafeObject()
    {
        PyAcquireLock pylock;
        delete _object;
        _object = 0;
    }

    PySafeObject(const PySafeObject &other)
        : _object(0)
    {
        PyAcquireLock pylock;
        _object = new T(*other._object);
    }

    const PySafeObject &
    operator = (const PySafeObject &other)
    {
        if (&other == this) return *this;
        PyAcquireLock pylock;
        *_object = *other._object;
        return *this;
    }

    bool
    operator == (const PySafeObject &other) const
    {
        if (&other == this) return true;
        PyAcquireLock pylock;
        return *_object == *other._object;
    }

    bool
    operator != (const PySafeObject &other) const
    {
        if (&other == this) return false;
        PyAcquireLock pylock;
        return *_object != *other._object;
    }

    T & get() { return *_object; }
    const T & get() const { return *_object; }

  private:

    T *_object;
};

/**
 * A special selectable postcall policy used in python wrappings.
 *
 * It expects the initial result to be a touple where the first
 * object represents an integer value 0, 1, 2 which corresponds
 * to which of the templated call polices should be applied.
 *
 * This postcall policy is modeled after a similar one defined 
 * in PyGeomParticleUtil.h of the PyGeomParticle project.
 *
 */
template <class policy0, class policy1, class policy2>
struct selectable_postcall_policy_from_tuple : policy0
{
    static PyObject *
    postcall (PyObject* args, PyObject* result)
    {
        if (!PyTuple_Check (result))
        {
            PyErr_SetString (PyExc_TypeError,
                             "selectable_postcall: retval was not a tuple");
            return 0;
        }
        if (PyTuple_Size(result) != 2)
        {
            PyErr_SetString (PyExc_IndexError,
                             "selectable_postcall: retval was not a tuple of length 2");
            return 0;
        }

        // borrowed references within the tuple
        PyObject* object0 = PyTuple_GetItem (result, 0);  // 'Choice' integer
        PyObject* object1 = PyTuple_GetItem (result, 1);  // The actual object

        if (!PyInt_Check (object0))
        {
            PyErr_SetString (PyExc_TypeError,
                             "selectable_postcall: tuple item 0 was not an integer choice");
            return 0;
        }

        const long usePolicy = PyInt_AsLong (object0);

        // ensure correct reference count for returned object and decref the tuple
        Py_INCREF (object1);
        Py_DECREF (result );

        if (usePolicy <= 0)
            return policy0::postcall (args, object1);
        else if (usePolicy == 1)
            return policy1::postcall (args, object1);
        else  // usePolicy >= 2
            return policy2::postcall (args, object1);
    }
};

} // namespace PyImath

#endif
