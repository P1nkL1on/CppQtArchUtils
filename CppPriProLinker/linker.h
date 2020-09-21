#ifndef LINKER_H
#define LINKER_H

#include <QStringList>
#include <QHash>
#include "ref_file.h"
#include "file_type.h"

class Linker
{
public:
    Linker() = default;
    void addFilePathes(const QStringList &absFilePathes);
    QString findFilePathForRef(
            const QString &origFilePath,
            const QString &refText) const;
    QString findFilePathForRef(
            const QString &origFilePath,
            const RefFile &ref) const;
protected:
    void addFolderByFile(const QString &fileInFolderPath);
    static FileType typeOfFile(const QString &filePath);
    static inline bool isFolder(const QString &path);
    static inline QString parentFolder(const QString &path);
    QString findFilePathByRefEnd(
            const QString &currentFolderPath,
            const QString &searchingForEnd,
            QStringList &folderBlackList) const;
    QHash<QString, QStringList> m_folderToFilePathesHash;
};

#endif // LINKER_H
