//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathTask_h_
#define _PyImathTask_h_

#include <cstddef>
#include "PyImathExport.h"

namespace PyImath {

struct Task
{
    PYIMATH_EXPORT virtual ~Task() {}
    PYIMATH_EXPORT virtual void execute(size_t start,size_t end) = 0;
    PYIMATH_EXPORT virtual void execute(size_t start,size_t end, int tid) {execute(start,end);}
};

struct WorkerPool
{
    PYIMATH_EXPORT virtual ~WorkerPool() {}
    PYIMATH_EXPORT virtual size_t workers() const = 0;
    PYIMATH_EXPORT virtual void dispatch(Task &task,size_t length) = 0;
    PYIMATH_EXPORT virtual bool inWorkerThread() const = 0;

    PYIMATH_EXPORT static WorkerPool *currentPool();
    PYIMATH_EXPORT static void setCurrentPool(WorkerPool *pool);
};

PYIMATH_EXPORT void dispatchTask(Task &task,size_t length);
PYIMATH_EXPORT size_t workers();

}

#endif
