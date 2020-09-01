#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "file_factory.h"
#include "thread_utils.h"
#include "linker.h"

class FileScanner
{
public:
    FileScanner(Linker *linker);
    ~FileScanner();
    enum ParsingMode { InBackground, WithEventLoop, WithDialog };
    void parseFiles(
            const QStringList &filePathes,
            ThreadUtils::foo onFinish = []{},
            ParsingMode mode = InBackground);
    void tryLinkRefs(
            QStringList &filesToParse);

    QStringList filePathesInDir(
            const QString &dirPath,
            const QStringList &nameFilters,
            const QStringList &nameIgnores) const;
    QStringList parsedFilePathes() const;
    File *parsedFile(const QString &filePath) const;
    QHash<RefFile, QString> parsedRefHash(const QString &filePath) const;
protected:
    ThreadWorker *createWorkerWithContext(const QStringList &filePathes);
    QStringList selectNotParsed(
            const QStringList &filePathes) const;
    bool isParsed(const QString &filePath) const;

    QHash<QString, FileFactory *> m_extToFileFactoryHash;
    QVector<FileFactory *> m_factories;
    Linker *m_linker = nullptr;
    QHash<QString, File *> m_pathToParsedFilesHash;
    QHash<QString, QHash<RefFile, QString>> m_pathToParsedRefHash;
};

#endif // FILESCANNER_H
