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


    // parse files at given filePathes
    // lookupFolder is a folder, file-tree from which
    // will be used to setup unresolved links between them
    void parseFiles(
            const QStringList &filePathes,
            const QString &lookupFolder,
            ThreadUtils::foo onFinish = []{},
            ParsingMode mode = InBackground);
    QStringList parsedFilePathes() const;
    File *parsedFile(const QString &filePath) const;
    QHash<RefFile, QString> parsedRefHash(const QString &filePath) const;

    void tryLinkRefs(QStringList &filesToParse);


    // Takes given files, e.g. {"*.h", "*.cpp", "*.pro", "*.pri"}
    void setNameFilters(const QStringList &nameFilters);
    // Ignore given files, e.g. {"build-", "moc_", "3rdparty"}
    void setNameIgnore(const QStringList &nameIgnore);

    // list all the files, which fits seted filters, ignores
    // works recursively
    QStringList filePathesInDir(const QString &dirPath) const;


protected:
    ThreadWorker *createWorkerWithContext(const QStringList &filePathes, const QStringList &lookupFilePathes);
    QStringList selectNotParsed(
            const QStringList &filePathes) const;
    bool isParsed(const QString &filePath) const;
    QStringList m_nameFilters;
    QStringList m_nameIgnores;

    QHash<QString, FileFactory *> m_extToFileFactoryHash;
    QVector<FileFactory *> m_factories;
    Linker *m_linker = nullptr;
    QHash<QString, File *> m_pathToParsedFilesHash;
    QHash<QString, QHash<RefFile, QString>> m_pathToParsedRefHash;
};

#endif // FILESCANNER_H
