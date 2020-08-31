#include "file_scanner.h"

#include <QDirIterator>
#include <QDebug>

#include "thread_worker_lambda.h"
#include "thread_handler_dialog.h"
#include "thread_utils.h"
#include "file_cpp_factory.h"
#include "file_pro_factory.h"
#include "linker.h"

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

QVector<File *> FileScanner::parseDir(
        const QString &dir,
        const QStringList &fileWildCards,
        QWidget *dialogParent,
        QStringList &pathes)
{
    QStringList filePathes;
    QVector<File *> files;

    Linker linker;
    QVector<QHash<RefFile, QString>> preLinkHashes;

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
            filePathes << filePath;
            ++filesCount;
        }
        files = QVector<File *>(filesCount, nullptr);
        preLinkHashes = QVector<QHash<RefFile, QString>>(filesCount);
        worker->setStepsTotal(filesCount);
        linker = Linker(filePathes);
    });
    worker->setStep([&](int i){
        const QString filePath = filePathes[i];
        const QFileInfo info(filePath);
        const QString ext = info.suffix();
        FileFactory *factory = m_extToFileFactoryHash.value(ext, nullptr);
        if (not factory)
            return QString("No factory found for %1!").arg(filePath);
        QString err;
        File *file = factory->read(filePath, err);
        if (not file or not err.isEmpty())
            return err;
        files[i] = file;

        // linkage
        const Linker::RefType refType =
                filePath.endsWith(".pri") or filePath.endsWith(".pro") ?
                    Linker::RefType::Pro: Linker::RefType::Cpp;
        for (const RefFile &ref : file->refs())
            if (not ref.isSystem){
                const QString refPath = linker.findFilePathForRef(filePath, ref.text, refType);
                if (refPath.isEmpty())
                    continue;
                preLinkHashes[i].insert(ref, refPath);
            }
        return QString();
    });
    worker->setFinish([&]{
        // link
        for (int fileInd = 0; fileInd < files.length(); ++fileInd){
            const QHash<RefFile, QString> &preLink = preLinkHashes[fileInd];
            for (const RefFile &ref : preLink.keys()){
                const QString filePath = preLink.value(ref);
                const int refFileInd = filePathes.indexOf(filePath);
                if (refFileInd < 0)
                    continue;
                files[fileInd]->setRef(ref, files[refFileInd]);
                qDebug() << ref.text << "     -->    " << filePathes[refFileInd];
            }
        }
        qDebug() << "Finished";
        qDebug() << "Failed:" << worker->isFailed();
        qDebug() << "Errors:" << worker->valuableErrors();
    });


    ThreadHandlerDialog h(dialogParent);
    h.setDialogTitle("Parsing files");
    ThreadUtils::runOnBackground(&h, worker);

    qDebug() << "parse dir finished";
    pathes = filePathes;
    return files;
}
