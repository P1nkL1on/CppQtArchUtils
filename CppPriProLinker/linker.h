#ifndef LINKER_H
#define LINKER_H

#include <QStringList>
#include <QHash>

class Linker
{
public:
    enum class RefType {Cpp, Pro};
    Linker() = default;
    Linker(const QStringList &absFilePathes);
    QString findFilePathForRef(
            const QString &filePath,
            const QString &refText,
            RefType refType) const;
protected:
    inline bool isFolder(const QString &path) const;
    inline QString parentFolder(const QString &path) const;
    void addFolderByFile(const QString &fileInFolderPath);
    QString findFilePathByRefEnd(
            const QString &currentFolderPath,
            const QString &searchingForEnd,
            QStringList &folderBlackList) const;
    QHash<QString, QStringList> folderToFilesPathesHash;
};

#endif // LINKER_H
