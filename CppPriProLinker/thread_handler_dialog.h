#ifndef THREADHANDLERDIALOG_H
#define THREADHANDLERDIALOG_H

#include "thread_handler.h"
#include "dialog_progress.h"

class ThreadHandlerDialog : public ThreadHandler
{
public:
    ThreadHandlerDialog(
            QWidget *dialogParent = nullptr,
            DialogProgress::ShowElements showDialogElements
            = DialogProgress::TimeAndProgressAndTerminate);
protected:
    void startWorker(ThreadWorker *worker) override;
    QWidget *m_dialogParent = nullptr;
    DialogProgress::ShowElements m_showDialogElements;
};

#endif // THREADHANDLERDIALOG_H
