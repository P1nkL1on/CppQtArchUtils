#include "thread_handler.h"

ThreadHandler::~ThreadHandler()
{
    freeThread();
}

bool ThreadHandler::tryRun(ThreadWorker *worker)
{
    if (not isThreadFree())
        return false;
    Q_ASSERT(not m_isWorking);
    m_isWorking = true;
    startWorker(worker);
    return true;
}

void ThreadHandler::freeThread()
{
    if (not m_thread)
        return;
    m_thread->quit();
    m_thread->deleteLater();
    m_thread = nullptr;
}

bool ThreadHandler::isThreadFree()
{
    if (isWorking())
        return false;
    m_thread = new QThread();
    return true;
}

bool ThreadHandler::isWorking() const
{
    return m_isWorking;
}

void ThreadHandler::startWorker(ThreadWorker *worker)
{
    worker->moveToThread(m_thread);
    QObject::connect(m_thread, &QThread::started, worker, &ThreadWorker::run);
    QObject::connect(worker, &ThreadWorker::finished, worker, [=]{
        delete worker;
        freeThread();
        m_isWorking = false;
    });
    m_thread->start();
}
