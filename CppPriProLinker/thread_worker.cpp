#include "thread_worker.h"

ThreadWorker::ThreadWorker(QObject *parent) :
    QObject(parent)
{
}

ThreadWorker::~ThreadWorker()
{
}

void ThreadWorker::setStepFailed(int step, const QString &errMessage)
{
    Q_ASSERT(0 <= step and step < m_stepsTotal);
    m_errMessages[step] = errMessage;

    // calculate if failed
    int stepsFailed = 0;
    for (const QString &err : m_errMessages){
        if (m_failPolicy == FailPolicy::IfAtLeastOneStepFailed){
            m_isFailed = true;
            break;
        }
        stepsFailed += err.isEmpty() * 1;
    }
    m_isFailed = m_failPolicy == FailPolicy::IfAllStepsFailed
            and stepsFailed == m_stepsTotal;
}

bool ThreadWorker::shouldSkipEachStep() const
{
    return isFailed();
}

void ThreadWorker::notifyStepProgress()
{
    ++m_stepsNotified;
    emit notifySteps(m_stepsNotified);
    emit notifyProgress(float(m_stepsNotified) / m_stepsTotal);
}

int ThreadWorker::stepsTotal() const
{
    return m_stepsTotal;
}

void ThreadWorker::setFailPolicy(const FailPolicy &failPolicy)
{
    m_failPolicy = failPolicy;
}

void ThreadWorker::run()
{
    emit started();

    start();

    // clear messages
    m_errMessages = QVector<QString>(m_stepsTotal);

    // #pragma omp parallel for
    for (int i = 0; i < m_stepsTotal; ++i){
        if (not shouldSkipEachStep()){
            const QString err = step(i);
            if (err.isEmpty())
                notifyStepProgress();
            else
                setStepFailed(i, err);
        }
    }

    finish();

    emit finished();
}

bool ThreadWorker::isFailed() const
{
    return m_isFailed;
}

void ThreadWorker::setStepsTotal(int steps)
{
    Q_ASSERT(steps > 0);
    m_stepsTotal = steps;
}

QVector<QString> ThreadWorker::errors() const
{
    return m_errMessages;
}

QMap<int, QString> ThreadWorker::valuableErrors() const
{
    QMap<int, QString> res;
    for (int i = 0; i < m_errMessages.length(); ++i)
        if (not m_errMessages[i].isEmpty())
            res.insert(i, m_errMessages[i]);
    return res;
}
