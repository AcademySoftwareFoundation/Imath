//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include "PyImathConfigInternal.h"

#include "PyImathTask.h"

namespace PyImath {

static WorkerPool *_currentPool = 0;

WorkerPool *
WorkerPool::currentPool()
{
    return _currentPool;
}

void
WorkerPool::setCurrentPool(WorkerPool *pool)
{
    _currentPool = pool;
}

void
dispatchTask(Task &task,size_t length)
{
    if (WorkerPool::currentPool() && !WorkerPool::currentPool()->inWorkerThread())
        WorkerPool::currentPool()->dispatch(task,length);
    else
        task.execute(0,length,0);
}


size_t
workers()
{
    if (WorkerPool::currentPool() && !WorkerPool::currentPool()->inWorkerThread())
        return WorkerPool::currentPool()->workers();
    else
        return 1;
}

}
