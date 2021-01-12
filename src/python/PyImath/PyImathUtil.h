//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef INCLUDED_PYIMATHUTIL_H
#define INCLUDED_PYIMATHUTIL_H

//----------------------------------------------------------------------------
//
//	PyImath.h -- miscellaneous classes, functions
//	and macros that are useful for Python wrapping
//	of C++ objects.
//
//----------------------------------------------------------------------------

#include "PyImathExport.h"
#include <Python.h>

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

} // namespace PyImath

#endif
