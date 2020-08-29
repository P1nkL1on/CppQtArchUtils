#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <QThread>

#include "thread_worker.h"

class ThreadHandler
{
public:
    ThreadHandler() = default;
    virtual ~ThreadHandler();
    bool tryRun(ThreadWorker *worker);
    bool isWorking() const;

protected:
    void cleanThreadIfExist(QThread *thread);
    QThread *tryGetFreeThread();
    bool m_isWorking = false;
    QThread *m_thread = nullptr;
};

#endif // THREADHANDLER_H
