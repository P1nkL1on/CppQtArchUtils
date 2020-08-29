#include "thread_worker_interruptable.h"

ThreadWorkerInterruptable::ThreadWorkerInterruptable(QObject *parent) :
    ThreadWorker(parent)
{

}

ThreadWorkerInterruptable::~ThreadWorkerInterruptable()
{
}

void ThreadWorkerInterruptable::abort()
{
    m_isAborted = true;
}

bool ThreadWorkerInterruptable::shouldSkipEachStep() const
{
    return isAborted() or isFailed();
}

bool ThreadWorkerInterruptable::isAborted() const
{
    return m_isAborted;
}
