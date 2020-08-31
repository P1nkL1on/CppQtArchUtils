#include "thread_handler_dialog.h"
#include "thread_worker_interruptable.h"

ThreadHandlerDialog::ThreadHandlerDialog(
        QWidget *dialogParent,
        DialogProgress::ShowElements showDialogElements) :
    ThreadHandler(),
    m_dialogParent(dialogParent),
    m_showDialogElements(showDialogElements)
{
}

void ThreadHandlerDialog::startWorker(ThreadWorker *worker)
{
    ThreadWorkerInterruptable *workerInter
            = qobject_cast<ThreadWorkerInterruptable *>(worker);
    const bool canBeTerminated = workerInter;
    DialogProgress::ShowElements flags = m_showDialogElements;
    if (not canBeTerminated)
        flags &= ~DialogProgress::TerminataBtn;
    DialogProgress *dialog = new DialogProgress(m_dialogParent, flags);
    if (not m_dialogTitle.isEmpty())
        dialog->setWindowTitle(m_dialogTitle);
    QObject::connect(worker, &ThreadWorker::notifySteps, dialog, [=](int progress){
        dialog->setTotal(worker->stepsTotal());
        dialog->setProgress(progress);
    });
    QObject::connect(worker, &ThreadWorker::finished,
                     dialog, &DialogProgress::accept);
    QObject::connect(dialog, &DialogProgress::terminated, [=]{
        if (canBeTerminated)
            workerInter->abort();
    });
    ThreadHandler::startWorker(worker);
    dialog->exec();
}

void ThreadHandlerDialog::setDialogTitle(const QString &dialogTitle)
{
    m_dialogTitle = dialogTitle;
}
