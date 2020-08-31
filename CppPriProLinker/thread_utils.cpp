#include "thread_utils.h"

//#include <QEventLoop>

//void ThreadUtils::runWithEventLoop(
//        ThreadHandler *handler,
//        ThreadWorker *worker)
//{
//    QEventLoop loop;
//    QObject::connect(worker, &ThreadWorker::finished,
//                     &loop, &QEventLoop::quit);
//    const bool isOk = handler->tryRun(worker);
//    Q_ASSERT(isOk);
//    loop.exec(QEventLoop::ExcludeUserInputEvents);
//}

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
