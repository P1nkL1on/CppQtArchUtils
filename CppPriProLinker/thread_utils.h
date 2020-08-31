#ifndef THREADUTILS_H
#define THREADUTILS_H

#include "thread_handler.h"
#include "thread_worker.h"

namespace ThreadUtils
{
using foo = std::function<void()>;

void runWithEventLoop(
        ThreadHandler *handler,
        ThreadWorker *worker);

void runOnBackground(
        ThreadHandler *handler,
        ThreadWorker *worker,
        foo onFinish = []{});
}

#endif // THREADUTILS_H
