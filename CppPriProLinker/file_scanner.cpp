#include "file_scanner.h"

#include <QDirIterator>
#include <QDebug>

#include "thread_worker_lambda.h"
#include "thread_handler_dialog.h"
#include "thread_utils.h"
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
        {"pro", pris}
    };
}

FileScanner::~FileScanner()
{
}

void FileScanner::parseDir(
        const QString &dir,
        const QStringList &fileWildCards,
        QWidget *dialogParent)
{
    QStringList filePathes;
    QVector<File *> files;
    QHash<QString, QStringList> folderToFilesPathesHash;
    const auto addFile = [&](const QString &filePath){
        filePathes << filePath;
    };

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
            const QString filePath = dirIterator.next();
            if (filePath.contains("moc_") or filePath.contains("build-"))
                continue;
            addFile(filePath);
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
            return QString("No factory found for %1!").arg(filePath);
        QString err;
        File * file = factory->read(filePath, err);
        if (not file or not err.isEmpty())
            return err;
        files[i] = file;
        return QString();
    });
    worker->setFinish([=]{
        qDebug() << "Finished";
        qDebug() << "Failed:" << worker->isFailed();
        qDebug() << "Errors:" << worker->valuableErrors();
    });


    ThreadHandlerDialog h(dialogParent);
    h.setDialogTitle("Parsing files");
    ThreadUtils::runOnBackground(&h, worker);

    qDebug() << "parse dir finished";
}
