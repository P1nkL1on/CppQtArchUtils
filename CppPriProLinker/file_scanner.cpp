#include "file_scanner.h"

#include <QDirIterator>
#include <QDebug>

#include "thread_worker_file_parser.h"
#include "thread_handler_dialog.h"
#include "file_cpp_factory.h"
#include "file_pro_factory.h"

FileScanner::FileScanner(Linker *linker) :
    m_linker(linker)
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
    for (FileFactory *f : m_factories)
        delete f;
}

QStringList FileScanner::filePathesInDir(const QString &dirPath) const
{
    QDirIterator dirIterator(
                dirPath,
                m_nameFilters,
                QDir::Files,
                QDirIterator::Subdirectories);
    QStringList res;
    while (dirIterator.hasNext()){
        const QString filePath = dirIterator.next();
        bool skipFile = false;
        for (const QString &badPath : m_nameIgnores)
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

void FileScanner::parseFiles(
        const QStringList &filePathes,
        const QString &lookupFolder,
        ThreadUtils::foo onFinish,
        FileScanner::ParsingMode mode)
{
    const QStringList notParsedFilePathes = selectNotParsed(filePathes);
    if (notParsedFilePathes.isEmpty())
        return;
    ThreadWorker *worker = createWorkerWithContext(notParsedFilePathes);
    Q_ASSERT(worker);
    ThreadHandler *threadHandler = nullptr;
    switch (mode){
    case WithDialog:
        threadHandler = new ThreadHandlerDialog;
        break;
    default:
        threadHandler = new ThreadHandler;
    }
    switch (mode){
    case WithEventLoop:
        ThreadUtils::runWithEventLoop(threadHandler, worker);
        onFinish();
        break;
    default:
        ThreadUtils::runOnBackground(threadHandler, worker, onFinish);
        break;
    }
}

QStringList FileScanner::selectNotParsed(const QStringList &filePathes) const
{
    QStringList res;
    for (const QString &p : filePathes)
        if (not isParsed(p))
            res << p;
    return res;
}


void FileScanner::tryLinkRefs(QStringList &filesToParse)
{
    const QStringList filePathes = m_pathToParsedFilesHash.keys();

    for (const QString &filePath : filePathes){
        Q_ASSERT(m_pathToParsedFilesHash.contains(filePath));
        File *file = m_pathToParsedFilesHash.value(filePath, nullptr);
        Q_ASSERT(file);
        QHash<RefFile, QString> &refHash = m_pathToParsedRefHash[filePath];
        const QList<RefFile> refs = refHash.keys();
        for (const RefFile &ref : refs){
            QString &refPath = refHash[ref];
            const bool wasNotResolvedYet = refPath.isEmpty();
            if (wasNotResolvedYet)
                refPath = m_linker->findFilePathForRef(filePath, ref);
            const bool stillNotResolved = refPath.isEmpty();
            if (stillNotResolved)
                continue;
            const bool noParsedFileToLink = not m_pathToParsedFilesHash.contains(refPath);
            if (noParsedFileToLink){
                filesToParse << refPath;
                continue;
            }
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

void FileScanner::setNameIgnore(const QStringList &nameIgnore)
{
    m_nameIgnores = nameIgnore;
}

void FileScanner::setNameFilters(const QStringList &nameFilters)
{
    m_nameFilters = nameFilters;
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

ThreadWorker *FileScanner::createWorkerWithContext(
        const QStringList &filePathes,
        const QStringList &lookupFilePathes)
{
    Q_ASSERT(not filePathes.isEmpty());
    ThreadWorkerFileParser *worker = new ThreadWorkerFileParser(
                m_extToFileFactoryHash,
                filePathes,
                lookupFilePathes);
    QObject::connect(worker, &ThreadWorker::finished, [=]{
        const QVector<File *> files = worker->files();
        const QVector<QHash<RefFile, QString>> refs = worker->refs();
        const int filesCount = filePathes.size();
        Q_ASSERT(files.size() == filesCount);
        Q_ASSERT(refs.size() == filesCount);

        for (int fileInd = 0; fileInd < filesCount; ++fileInd){
            if (not files.at(fileInd))
                continue;
            const QString parsingFilePath = filePathes.at(fileInd);
            m_pathToParsedFilesHash.insert(parsingFilePath, files.at(fileInd));
            m_pathToParsedRefHash.insert(parsingFilePath, refs.at(fileInd));
        }
    });
    return worker;
}
