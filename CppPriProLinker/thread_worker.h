#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include <QVector>
#include <QMap>

class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    enum class FailPolicy {
        IfAtLeastOneStepFailed,
        IfAllStepsFailed,
        Never
    };
    explicit ThreadWorker(QObject *parent = nullptr);
    virtual ~ThreadWorker();
    QVector<QString> errors() const;
    QMap<int, QString> valuableErrors() const;
    bool isFailed() const;
    void setStepsTotal(int steps);
    void setFailPolicy(const FailPolicy &failPolicy);
    int stepsTotal() const;
    int successStepsCount() const;
public slots:
    void run();
protected slots:
    void setStepFailed(int step, const QString &errors);
signals:
    void finished();
    void stepSuccess(int stedInd);
    void stepFail(int stedInd);
    void notifySteps(int steps);
    void notifyProgress(float progress);
    void started();
protected:
    virtual bool shouldSkipEachStep() const;
    void notifyStepProgress();
    virtual void start() = 0;
    virtual QString step(int) = 0;
    virtual void finish() = 0;

    int m_stepsTotal = 1;
    int m_stepsNotified = 0;
    QVector<QString> m_errMessages;
    bool m_isFailed = false;
    FailPolicy m_failPolicy = FailPolicy::Never;

};

#endif // THREADWORKER_H
