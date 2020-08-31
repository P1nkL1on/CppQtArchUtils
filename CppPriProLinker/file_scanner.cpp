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

QVector<File *> FileScanner::parseDir(
        const QString &dir,
        const QStringList &fileWildCards,
        QWidget *dialogParent)
{
    QStringList filePathes;
    QVector<File *> files;
    QHash<QString, QStringList> folderToFilesPathesHash;
    QVector<QHash<RefFile, QString>> preLinkHashes;


    const auto isFolder = [](const QString &path) -> bool { return path.contains("/"); };
    const auto folder = [](const QString &path) -> QString { return path.mid(0, path.lastIndexOf("/")); };

    const std::function<void(const QString &)> addFolder = [&](const QString &fileInFolderPath){
        if (not isFolder(fileInFolderPath))
            return;
        const QString folderPath = folder(fileInFolderPath);
        if (not folderToFilesPathesHash.contains(folderPath))
            addFolder(folderPath);
        folderToFilesPathesHash[folderPath] << fileInFolderPath;
    };
    const auto addFile = [&](const QString &filePath){
        filePathes << filePath;
        addFolder(filePath);
    };
    const std::function<QString (const QString &, const QString &, QStringList &)> folderLink
            = [&](const QString &folderPath, const QString &fileEnd, QStringList &folderBlackList) -> QString
    {
        const QStringList filePathesInFolder = folderToFilesPathesHash.value(folderPath);
        QStringList foldersToCheck {folder(folderPath)};
        for (const QString &p : filePathesInFolder){
            if (folderToFilesPathesHash.contains(p))
                foldersToCheck << p;
            else if (p.endsWith(fileEnd))
                return p;
        }
        folderBlackList << folderPath;
        for (const QString &f : foldersToCheck)
            if (not folderBlackList.contains(f)){
                const QString res = folderLink(f, fileEnd, folderBlackList);
                if (not res.isEmpty())
                    return res;
            }
        return QString();
    };
    const auto resolveLinkPath = [=](const QString &filePath, const RefFile &ref) -> QString {
        QString t = ref.text;
        if (not t.startsWith("/"))
            t = "/" + t;
        QStringList b;
        return folderLink(folder(filePath), t, b);
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
        preLinkHashes = QVector<QHash<RefFile, QString>>(filesCount);
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
        File *file = factory->read(filePath, err);
        if (not file or not err.isEmpty())
            return err;
        files[i] = file;

        // linkage
        for (const RefFile &ref : file->refs())
            if (not ref.isSystem)
                preLinkHashes[i].insert(ref, resolveLinkPath(filePath, ref));
        return QString();
    });
    worker->setFinish([&]{
        // link
        for (int i = 0; i < files.length(); ++i){
            const QHash<RefFile, QString> &preLink = preLinkHashes[i];
            for (const RefFile &ref : preLink.keys()){
                const QString filePath = preLink.value(ref);
                const int j = filePathes.indexOf(filePath);
                if (j < 0)
                    continue;
                files[i]->setRef(ref, files[j]);
                qDebug() << ref.text << "     -->    " << filePathes[j];
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
    return files;
}
