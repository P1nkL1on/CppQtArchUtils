#ifndef THREADWORKERLAMBDA_H
#define THREADWORKERLAMBDA_H

#include <functional>

#include "thread_worker_interruptable.h"

class ThreadWorkerLambda : public ThreadWorkerInterruptable
{
    Q_OBJECT
public:
    using ProcessNoRet = std::function<void()>;
    using ProcessErrRet = std::function<QString()>;
    explicit ThreadWorkerLambda(QObject *parent = nullptr);
    void setStart(const ProcessNoRet &start);
    void setStep(const ProcessErrRet &step);
    void setFinish(const ProcessNoRet &finish);
protected:
    void start() override;
    QString step() override;
    void finish() override;
    ProcessNoRet m_start = nullptr;
    ProcessErrRet m_step = nullptr;
    ProcessNoRet m_finish = nullptr;
};

#endif // THREADWORKERLAMBDA_H
