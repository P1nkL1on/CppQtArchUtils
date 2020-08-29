#ifndef THREADWORKERINTERRUPTABLE_H
#define THREADWORKERINTERRUPTABLE_H

#include "thread_worker.h"

class ThreadWorkerInterruptable : public ThreadWorker
{
    Q_OBJECT
public:
    ThreadWorkerInterruptable(QObject *parent = nullptr);
    ~ThreadWorkerInterruptable() override;
    bool isAborted() const;
public slots:
    void abort();
protected:
    bool shouldSkipEachStep() const override;
    bool m_isAborted = false;
};

#endif // THREADWORKERINTERRUPTABLE_H
