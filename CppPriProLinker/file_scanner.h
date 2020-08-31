#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "file_factory.h"
#include "thread_utils.h"
#include "linker.h"

class FileScanner
{
public:
    FileScanner(
            const QStringList &fileWildCards,
            const QStringList &ignoreFilesMasks);
    ~FileScanner();

    void addLinkerFilePathes(const QStringList &filePathes);
    void parseFiles(const QStringList &filePathes, ThreadUtils::foo onFinish = []{});
    void link(QStringList &filesToParse);

    QStringList filePathesInDir(const QString &dirPath) const;
    QStringList parsedFilePathes() const;
    File *parsedFile(const QString &filePath) const;
    QHash<RefFile, QString> parsedRefHash(const QString &filePath) const;
protected:
    ThreadWorker *createParseFilesWorkerWithContext(
            const QStringList &filePathes);
    QString findFilePathByRef(
            const RefFile &ref,
            const QString &originFilePath) const;
    QStringList selectNotParsed(
            const QStringList &filePathes) const;
    bool isParsed(const QString &filePath) const;

    QStringList m_fileWildCards;
    QStringList m_ignoreFilesMasks;
    QHash<QString, FileFactory *> m_extToFileFactoryHash;
    QVector<FileFactory *> m_factories;
    Linker m_linker;
    QHash<QString, File *> m_pathToParsedFilesHash;
    QHash<QString, QHash<RefFile, QString>> m_pathToParsedRefHash;
    ThreadHandler *m_threadHandler = nullptr;
};

#endif // FILESCANNER_H
