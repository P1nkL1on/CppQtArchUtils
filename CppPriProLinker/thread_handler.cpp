#include "thread_handler.h"

ThreadHandler::~ThreadHandler()
{
    cleanThreadIfExist(m_thread);
}

bool ThreadHandler::tryRun(ThreadWorker *worker)
{
    QThread *thread = tryGetFreeThread();
    if (not thread)
        return false;
    Q_ASSERT(not m_isWorking);
    m_isWorking = true;
    QObject::connect(thread, &QThread::started, worker, &ThreadWorker::run);
    QObject::connect(worker, &ThreadWorker::finished, worker, [=]{
        m_isWorking = false;
        delete worker;
        cleanThreadIfExist(thread);
    });
    return true;
}

void ThreadHandler::cleanThreadIfExist(QThread *thread)
{
    thread->quit();
    thread->wait();
    delete thread;
}

QThread *ThreadHandler::tryGetFreeThread()
{
    if (isWorking())
        return nullptr;
    m_thread = new QThread();
    return m_thread;
}

bool ThreadHandler::isWorking() const
{
    return m_isWorking;
}
