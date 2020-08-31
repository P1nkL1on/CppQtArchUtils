#include "thread_utils.h"

#include <QEventLoop>
#include "thread_handler_dialog.h"

void ThreadUtils::runWithEventLoop(
        ThreadHandler *handler,
        ThreadWorker *worker)
{
    Q_ASSERT_X(not dynamic_cast<ThreadHandlerDialog *>(handler),
               "ThreadUtils::runWithEventLoop",
               "Use this method only to call "
               "handler without inner event loop!");
    QEventLoop loop;
    QObject::connect(worker, &ThreadWorker::finished,
                     &loop, &QEventLoop::quit);
    const bool isOk = handler->tryRun(worker);
    Q_ASSERT(isOk);
    loop.exec(QEventLoop::ExcludeUserInputEvents);
}

void ThreadUtils::runOnBackground(
        ThreadHandler *handler,
        ThreadWorker *worker,
        ThreadUtils::foo onFinish)
{
    QObject::connect(worker, &ThreadWorker::finished,
                     worker, onFinish);
    const bool isOk = handler->tryRun(worker);
    Q_ASSERT(isOk);
}
