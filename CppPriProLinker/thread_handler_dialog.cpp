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
            = dynamic_cast<ThreadWorkerInterruptable *>(worker);
    const bool canBeTerminated = workerInter;
    DialogProgress::ShowElements flags = m_showDialogElements;
    if (not canBeTerminated)
        flags &= ~DialogProgress::TerminataBtn;
    DialogProgress *dialog = new DialogProgress(m_dialogParent, flags);
    QObject::connect(worker, &ThreadWorker::started,
                     dialog, [=]{ dialog->setTotal(worker->stepsTotal()); });
    QObject::connect(worker, &ThreadWorker::notifySteps,
                     dialog, &DialogProgress::setProgress);
    QObject::connect(worker, &ThreadWorker::finished,
                     dialog, &DialogProgress::accept);
    if (canBeTerminated)
        QObject::connect(dialog, &DialogProgress::terminated,
                         workerInter, &ThreadWorkerInterruptable::abort);
    ThreadHandler::startWorker(worker);
    dialog->exec();
}
