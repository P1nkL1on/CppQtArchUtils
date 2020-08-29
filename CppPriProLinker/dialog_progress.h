#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include "dialog.h"

class DialogProgress : public Dialog
{
    Q_OBJECT
public:
    enum Show {
        None            = 0,
        StateLabel      = 1,
        ElapsedTime     = 2,
        RemainingTime   = 4,
        ProgressBar     = 8,
        TerminataBtn    = 16,
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
protected:
    void closeEvent(QCloseEvent *e) override;
    bool m_isTerminated = false;
};

#endif // DIALOGPROGRESS_H
