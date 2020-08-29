#include "file_scanner.h"

#include <QDirIterator>
#include <QDebug>
#include <QEventLoop>

#include "thread_worker_lambda.h"
#include "thread_handler_dialog.h"
#include "file_cpp_factory.h"
#include "file_pro_factory.h"

FileScanner::FileScanner()
{
    auto pris = new FileProFactory;
    auto cpps = new FileCppFactory;
    m_extToFileFactoryHash = {
        {"h", cpps},
        {"cpp", cpps},
        {"pri", pris},
//        {"pro", pris}
    };
}

FileScanner::~FileScanner()
{
}

void FileScanner::parseDir(
        const QString &dir,
        const QStringList &fileWildCards,
        bool showDialog,
        QWidget *dialogParent)
{
    QStringList filePathes;
    QVector<File *> files;

    // worker definition
    ThreadWorkerLambda *worker = new ThreadWorkerLambda;
    worker->setFailPolicy(ThreadWorker::FailPolicy::IfAllStepsFailed);
    worker->setStart([&]{
        QDirIterator dirIterator(
                    dir,
                    fileWildCards,
                    QDir::Files,
                    QDirIterator::Subdirectories);
        int filesCount = 0;
        while (dirIterator.hasNext()){
            filePathes << dirIterator.next();
            ++filesCount;
        }
        files = QVector<File *>(filesCount, nullptr);
        worker->setStepsTotal(filesCount);
    });
    worker->setStep([&](int i){
        const QString filePath = filePathes[i];
        const QFileInfo info(filePath);
        const QString ext = info.suffix();
        FileFactory *factory = m_extToFileFactoryHash.value(ext, nullptr);
        if (not factory)
            return QString("No factory found for %1!")
                    .arg(filePath);
        QString err;
        File * file = FileScanner::parseFile(filePath, factory, err);
        if (not file or not err.isEmpty())
            return err;
        files[i] = file;
        qDebug() << "OK    " << filePath;
        return QString();
    });
    worker->setFinish([=]{
        qDebug() << "Finished";
        qDebug() << "Failed:" << worker->isFailed();
        qDebug() << "Errors:" << worker->valuableErrors();
    });

    QEventLoop loop;
    QObject::connect(worker, &ThreadWorker::finished,
                     &loop, &QEventLoop::quit);
    ThreadHandler h;
    const bool isOk = h.tryRun(worker);
    Q_ASSERT(isOk);
    loop.exec(QEventLoop::ExcludeUserInputEvents);

    qDebug() << "parse dir finished";
    qDeleteAll(files);
}

File *FileScanner::parseFile(
        const QString &filePath,
        const FileFactory *reader,
        QString &err)
{
    const QString ext = QFileInfo(filePath).suffix();
    return reader->read(filePath, err);
}
