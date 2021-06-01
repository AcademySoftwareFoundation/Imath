//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <boost/python.hpp>
#include <pystate.h>
#include "PyImathUtil.h"

namespace PyImath {

PyAcquireLock::PyAcquireLock()
{
    _gstate = PyGILState_Ensure();
}

PyAcquireLock::~PyAcquireLock()
{
    PyGILState_Release(_gstate);
}

#ifdef PLATFORM_LINUX
// On Windows, this extern is not needed and produces a symbol mismatch at link time.
// We should verify that it's still needed on Linux for Python 2.6.
extern "C" PyThreadState *_PyThreadState_Current;
#endif

static bool
pyHaveLock()
{
#if PY_MAJOR_VERSION > 2
    return PyGILState_Check() != 0;
#else
    // This is very much dependent on the current Python
    // implementation of this functionality.  If we switch versions of
    // Python and the implementation changes, we'll have to change
    // this code as well and introduce a #define for the Python
    // version.
    
    if (!Py_IsInitialized())
      throw std::invalid_argument ("PyReleaseLock called without the interpreter initialized");

    PyThreadState *myThreadState = PyGILState_GetThisThreadState();

    // If the interpreter is initialized the gil is held if the
    // current thread's thread state is the current thread state
    return myThreadState != 0 && myThreadState == _PyThreadState_Current;
#endif
}

PyReleaseLock::PyReleaseLock()
{
    // only call PyEval_SaveThread if we have the interpreter lock held,
    // otherwise PyReleaseLock is a no-op.
    if (pyHaveLock())
        _save = PyEval_SaveThread();
    else
        _save = 0;
}

PyReleaseLock::~PyReleaseLock()
{
    if (_save != 0)
        PyEval_RestoreThread(_save);
}

} // namespace PyImath
