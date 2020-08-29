#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QLabel>
#include <QProgressBar>
#include <QElapsedTimer>
#include <QTimer>

#include "dialog.h"

class DialogProgress : public Dialog
{
    Q_OBJECT
public:
    enum Show {
        None            = 0,
        ElapsedTime     = 1,
        RemainingTime   = 2,
        ProgressBar     = 4,
        TerminataBtn    = 8,
        Time            = ElapsedTime | RemainingTime,
        TimeAndProgress = Time | ProgressBar,
        TimeAndProgressAndTerminate = TimeAndProgress | TerminataBtn
    };
    Q_DECLARE_FLAGS(ShowElements, Show)

    explicit DialogProgress(
            QWidget *parent = nullptr,
            ShowElements s = TimeAndProgressAndTerminate,
            Qt::WindowFlags f = Qt::WindowFlags());
    ~DialogProgress() override;
    void setTotal(int total);
public slots:
    void setProgress(int progress);
    void reject() override;
    int exec() override;
signals:
    void terminated();
protected:
    void closeEvent(QCloseEvent *e) override;
    QString formatTime(int64_t ms) const;
    ShowElements m_shownElements;
    int m_total = 1;
    int m_progress = 0;
    bool m_isTerminated = false;

    QLabel *m_labelElapsed = nullptr;
    QLabel *m_labelRemaining = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QElapsedTimer m_elaspedTimer;
    QTimer *m_timer = nullptr;
};

#endif // DIALOGPROGRESS_H
