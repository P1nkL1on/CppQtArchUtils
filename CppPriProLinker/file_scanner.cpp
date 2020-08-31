#include "file_scanner.h"

#include <QDirIterator>
#include <QDebug>

#include "thread_worker_lambda.h"
#include "thread_handler_dialog.h"
#include "thread_utils.h"
#include "file_cpp_factory.h"
#include "file_pro_factory.h"

FileScanner::FileScanner(
        const QStringList &fileWildCards,
        const QStringList &ignoreFilesMasks) :
    m_fileWildCards(fileWildCards),
    m_ignoreFilesMasks(ignoreFilesMasks)
{
    auto pros = new FileProFactory;
    auto cpps = new FileCppFactory;
    m_factories = {
        pros, cpps
    };
    m_extToFileFactoryHash = {
        {"h", cpps},
        {"cpp", cpps},
        {"pri", pros},
        {"pro", pros}
    };
}

FileScanner::~FileScanner()
{
}

QStringList FileScanner::scanDir(
        const QString &dirPath) const
{
    QDirIterator dirIterator(
                dirPath,
                m_fileWildCards,
                QDir::Files,
                QDirIterator::Subdirectories);
    QStringList res;
    while (dirIterator.hasNext()){
        const QString filePath = dirIterator.next();
        bool skipFile = false;
        for (const QString &badPath : m_ignoreFilesMasks)
            if (filePath.contains(badPath)){
                skipFile = true;
                break;
            }
        if (skipFile)
            continue;
        res << filePath;
    }
    return res;
}

void FileScanner::addScannedFilePathes(const QStringList &files)
{
    m_scannedPathes << files;
    m_linker.addFiles(files);
}

int FileScanner::cacheFiles(const QStringList &filePathes)
{
    QStringList pathes;
    for (const QString &p : filePathes)
        if (not m_pathToParsedFilesHash.contains(p))
            pathes << p;
    if (pathes.isEmpty())
        return 0;

    m_linker.addFiles(pathes);
    const int filesCount = pathes.size();

    ThreadWorkerLambda *worker = new ThreadWorkerLambda;
    worker->setFailPolicy(ThreadWorker::FailPolicy::IfAllStepsFailed);
    worker->setStepsTotal(filesCount);

    QVector<File *> files(filesCount, nullptr);
    QVector<QHash<RefFile, QString>> preLinks(filesCount);
    worker->setStep([&](int i){
        const QString filePath = pathes[i];
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
        for (const RefFile &ref : file->refs())
            preLinks[i].insert(ref, preLinkRef(ref, filePath));
        return QString();
    });
    QVector<QString> errors;
    int failedCount = 0;
    worker->setFinish([&]{
        errors = worker->errors();
        failedCount = worker->valuableErrors().count();
    });

    ThreadHandlerDialog h(nullptr);
    h.setDialogTitle("Parsing files");
    ThreadUtils::runOnBackground(&h, worker);
    qDebug() << "parsed:" << (filesCount - failedCount) << "failed:" << failedCount;

    for (int fileInd = 0; fileInd < filesCount; ++fileInd){
        // do not cache errored files
        if (not files[fileInd])
            continue;
        m_pathToParsedFilesHash.insert(pathes[fileInd], files[fileInd]);
        m_pathToPreLinkHash.insert(pathes[fileInd], preLinks[fileInd]);
    }
    return filesCount - failedCount;
}

QString FileScanner::preLinkRef(const RefFile &ref, const QString &filePath) const
{
    if (ref.isSystem)
        return QString();
    const Linker::RefType refType = filePath.endsWith(".pri") or filePath.endsWith(".pro") ?
                Linker::RefType::Pro: Linker::RefType::Cpp;
    const QString res = m_linker.findFilePathForRef(filePath, ref.text, refType);
    return res;
}


void FileScanner::link(QStringList &filesToParse)
{
    const QStringList filePathes = m_pathToParsedFilesHash.keys();

    for (const QString &filePath : filePathes){
        Q_ASSERT(m_pathToParsedFilesHash.contains(filePath));
        File *file = m_pathToParsedFilesHash.value(filePath, nullptr);
        Q_ASSERT(file);
        QHash<RefFile, QString> &preLinkHash = m_pathToPreLinkHash[filePath];
        const QList<RefFile> refs = preLinkHash.keys();
        for (const RefFile &ref : refs){
            QString &refPath = preLinkHash[ref];
            const bool wasNotResolvedYet = refPath.isEmpty();
            if (wasNotResolvedYet)
                refPath = preLinkRef(ref, filePath);
            const bool stillNotResolved = refPath.isEmpty();
            if (stillNotResolved)
                continue;
            const bool noParsedFileToLink = not m_pathToParsedFilesHash.contains(refPath);
            if (noParsedFileToLink){
                filesToParse << refPath;
                continue;
            }
            // preLinkHash.remove(ref);
            File *resolvedRef = m_pathToParsedFilesHash.value(refPath, nullptr);
            Q_ASSERT(resolvedRef);
            file->setRef(ref, resolvedRef);
        }
    }
}

bool FileScanner::isCached(
        const QString &filePath) const
{
    return m_pathToPreLinkHash.contains(filePath);
}

QStringList FileScanner::scannedFilePathes() const
{
    return m_scannedPathes;
}

File *FileScanner::cachedFile(
        const QString &filePath) const
{
    Q_ASSERT(isCached(filePath));
    return m_pathToParsedFilesHash.value(filePath, nullptr);
}

QHash<RefFile, QString> FileScanner::preLinkHash(
        const QString &filePath) const
{
    Q_ASSERT(isCached(filePath));
    return m_pathToPreLinkHash.value(filePath);
}
//QVector<File *> FileScanner::parseDir(
//        const QString &dir,
//        const QStringList &fileWildCards,
//        QWidget *dialogParent,
//        QStringList &pathes)
//{
//    QStringList filePathes;
//    QVector<File *> files;

//    Linker linker;
//    QVector<QHash<RefFile, QString>> preLinkHashes;

//    ThreadWorkerLambda *worker = new ThreadWorkerLambda;
//    worker->setFailPolicy(ThreadWorker::FailPolicy::IfAllStepsFailed);
//    worker->setStart([&]{
//        QDirIterator dirIterator(
//                    dir,
//                    fileWildCards,
//                    QDir::Files,
//                    QDirIterator::Subdirectories);
//        int filesCount = 0;
//        while (dirIterator.hasNext()){
//            const QString filePath = dirIterator.next();
//            if (filePath.contains("moc_") or filePath.contains("build-"))
//                continue;
//            filePathes << filePath;
//            ++filesCount;
//        }
//        files = QVector<File *>(filesCount, nullptr);
//        preLinkHashes = QVector<QHash<RefFile, QString>>(filesCount);
//        worker->setStepsTotal(filesCount);
//        linker.addFiles(filePathes);
//    });
//    worker->setStep([&](int i){
//        const QString filePath = filePathes[i];
//        const QFileInfo info(filePath);
//        const QString ext = info.suffix();
//        FileFactory *factory = m_extToFileFactoryHash.value(ext, nullptr);
//        if (not factory)
//            return QString("No factory found for %1!").arg(filePath);
//        QString err;
//        File *file = factory->read(filePath, err);
//        if (not file or not err.isEmpty())
//            return err;
//        files[i] = file;

//        // linkage
//        const Linker::RefType refType =
//                filePath.endsWith(".pri") or filePath.endsWith(".pro") ?
//                    Linker::RefType::Pro: Linker::RefType::Cpp;
//        for (const RefFile &ref : file->refs())
//            if (not ref.isSystem){
//                const QString refPath = linker.findFilePathForRef(filePath, ref.text, refType);
//                if (refPath.isEmpty())
//                    continue;
//                preLinkHashes[i].insert(ref, refPath);
//            }
//        return QString();
//    });
//    worker->setFinish([&]{
//        // link
//        for (int fileInd = 0; fileInd < files.length(); ++fileInd){
//            const QHash<RefFile, QString> &preLink = preLinkHashes[fileInd];
//            for (const RefFile &ref : preLink.keys()){
//                const QString filePath = preLink.value(ref);
//                const int refFileInd = filePathes.indexOf(filePath);
//                if (refFileInd < 0)
//                    continue;
//                files[fileInd]->setRef(ref, files[refFileInd]);
//            }
//        }
//        qDebug() << "Finished";
//        qDebug() << "Failed:" << worker->isFailed();
//        qDebug() << "Errors:" << worker->valuableErrors();
//    });


//    ThreadHandlerDialog h(dialogParent);
//    h.setDialogTitle("Parsing files");
//    ThreadUtils::runOnBackground(&h, worker);

//    qDebug() << "parse dir finished";
//    pathes = filePathes;
//    return files;
//}
