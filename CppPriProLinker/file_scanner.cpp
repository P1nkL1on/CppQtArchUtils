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
    m_threadHandler = new ThreadHandler;
}

FileScanner::~FileScanner()
{
    for (FileFactory *f : m_factories)
        delete f;
    delete m_threadHandler;
}

void FileScanner::addLinkerFilePathes(const QStringList &filePathes)
{
    m_linker.addFiles(filePathes);
}


QStringList FileScanner::filePathesInDir(
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

QStringList FileScanner::parsedFilePathes() const
{
    return m_pathToParsedFilesHash.keys();
}

void FileScanner::parseFiles(const QStringList &filePathes, ThreadUtils::foo onFinish)
{
    Q_ASSERT(not m_threadHandler->isWorking());
    const QStringList nonParsedFilePathes = selectNotParsed(filePathes);
    if (nonParsedFilePathes.isEmpty())
        return;
    addLinkerFilePathes(nonParsedFilePathes);
    ThreadWorker *worker = createParseFilesWorkerWithContext(nonParsedFilePathes);
    ThreadUtils::runOnBackground(m_threadHandler, worker, onFinish);
}

QString FileScanner::findFilePathByRef(
        const RefFile &ref,
        const QString &originFilePath) const
{
    if (ref.isSystem)
        return QString();
    const Linker::RefType refType = originFilePath.endsWith(".pri") or originFilePath.endsWith(".pro") ?
                Linker::RefType::Pro: Linker::RefType::Cpp;
    const QString res = m_linker.findFilePathForRef(originFilePath, ref.text, refType);
    return res;
}

QStringList FileScanner::selectNotParsed(const QStringList &filePathes) const
{
    QStringList res;
    for (const QString &p : filePathes)
        if (not isParsed(p))
            res << p;
    return res;
}


void FileScanner::link(QStringList &filesToParse)
{
    const QStringList filePathes = m_pathToParsedFilesHash.keys();

    for (const QString &filePath : filePathes){
        Q_ASSERT(m_pathToParsedFilesHash.contains(filePath));
        File *file = m_pathToParsedFilesHash.value(filePath, nullptr);
        Q_ASSERT(file);
        QHash<RefFile, QString> &preLinkHash = m_pathToParsedRefHash[filePath];
        const QList<RefFile> refs = preLinkHash.keys();
        for (const RefFile &ref : refs){
            QString &refPath = preLinkHash[ref];
            const bool wasNotResolvedYet = refPath.isEmpty();
            if (wasNotResolvedYet)
                refPath = findFilePathByRef(ref, filePath);
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

bool FileScanner::isParsed(const QString &filePath) const
{
    return m_pathToParsedRefHash.contains(filePath);
}

File *FileScanner::parsedFile(const QString &filePath) const
{
    Q_ASSERT(isParsed(filePath));
    return m_pathToParsedFilesHash.value(filePath, nullptr);
}

QHash<RefFile, QString> FileScanner::parsedRefHash(const QString &filePath) const
{
    Q_ASSERT(isParsed(filePath));
    return m_pathToParsedRefHash.value(filePath);
}

ThreadWorker *FileScanner::createParseFilesWorkerWithContext(const QStringList &filePathes)
{
    Q_ASSERT(not filePathes.isEmpty());
    QStringList *parsingFilePathes = new QStringList;
    *parsingFilePathes = filePathes;
    const int filesCount = parsingFilePathes->size();

    auto parsingFiles = new QVector<File *>(filesCount, nullptr);
    auto parsingRefHashes = new QVector<QHash<RefFile, QString>>(filesCount);
    auto worker = new ThreadWorkerLambda;
    worker->setFailPolicy(ThreadWorker::FailPolicy::IfAllStepsFailed);
    worker->setStepsTotal(filesCount);

    worker->setStep([=](int i){
        const QString filePath = parsingFilePathes->at(i);
        const QFileInfo info(filePath);
        const QString ext = info.suffix();
        FileFactory *factory = m_extToFileFactoryHash.value(ext, nullptr);
        if (not factory)
            return QString("No factory found for %1!").arg(filePath);
        QString err;
        File *file = factory->read(filePath, err);
        if (not file or not err.isEmpty())
            return err;
        parsingFiles->replace(i, file);
        for (const RefFile &ref : file->refs()){
            QHash<RefFile, QString> &hash = (*parsingRefHashes)[i];
            hash.insert(ref, findFilePathByRef(ref, filePath));
        }
        qDebug() << "ok" << filePath;
        return QString();
    });
    worker->setFinish([=]{
        const QVector<QString> errors = worker->errors();
        for (int fileInd = 0; fileInd < filesCount; ++fileInd){
            // do not cache nullptr files
            // which means has error
            if (not parsingFiles->at(fileInd))
                continue;
            const QString parsingFilePath = parsingFilePathes->at(fileInd);
            m_pathToParsedFilesHash.insert(parsingFilePath, parsingFiles->at(fileInd));
            m_pathToParsedRefHash.insert(parsingFilePath, parsingRefHashes->at(fileInd));
        }
        delete parsingFilePathes;
        for (File *file : *parsingFiles)
            if (file) delete file;
        delete parsingFiles;
        delete parsingRefHashes;
        Q_ASSERT(not m_pathToParsedFilesHash.contains(nullptr));
    });
    return worker;
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
