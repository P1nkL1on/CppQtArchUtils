#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "file_factory.h"
#include "thread_worker.h"
#include "linker.h"

class FileScanner
{
public:
    FileScanner(
            const QStringList &fileWildCards = {"*.pro", "*.pri", "*.h", "*.cpp"},
            const QStringList &ignoreFilesMasks = {"build-", "moc_"});
    ~FileScanner();

    void addScannedFilePathes(const QStringList &files);
    int cacheFiles(const QStringList &filePathes);
    void link(QStringList &filesToParse);

    QStringList scanDir(const QString &dirPath) const;

    QStringList scannedFilePathes() const;
    bool isCached(const QString &filePath) const;
    File *cachedFile(const QString &filePath) const;
    QHash<RefFile, QString> preLinkHash(const QString &filePath) const;

protected:
    QString preLinkRef(const RefFile &ref, const QString &filePath) const;
    QStringList m_fileWildCards;
    QStringList m_ignoreFilesMasks;
    QHash<QString, FileFactory *> m_extToFileFactoryHash;
    QVector<FileFactory *> m_factories;
    QStringList m_scannedPathes;

    Linker m_linker;

    QHash<QString, File *> m_pathToParsedFilesHash;
    QHash<QString, QHash<RefFile, QString>> m_pathToPreLinkHash;
};

#endif // FILESCANNER_H
