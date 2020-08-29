#include "thread_worker_lambda.h"

ThreadWorkerLambda::ThreadWorkerLambda(QObject *parent) :
    ThreadWorkerInterruptable(parent)
{
}

void ThreadWorkerLambda::start()
{
    if (m_start)
        m_start();
}

QString ThreadWorkerLambda::step()
{
    return m_step ? m_step() : QString();
}

void ThreadWorkerLambda::finish()
{
    if (m_finish)
        m_finish();
}

void ThreadWorkerLambda::setFinish(const ProcessNoRet &finish)
{
    m_finish = finish;
}

void ThreadWorkerLambda::setStep(const ProcessErrRet &step)
{
    m_step = step;
}

void ThreadWorkerLambda::setStart(const ProcessNoRet &start)
{
    m_start = start;
}
