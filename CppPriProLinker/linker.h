#ifndef LINKER_H
#define LINKER_H

#include <QStringList>
#include <QHash>

class Linker
{
public:
    enum class RefType {Cpp, Pro};
    Linker() = default;
    void addFiles(const QStringList &absFilePathes);
    QString findFilePathForRef(
            const QString &filePath,
            const QString &refText,
            RefType refType) const;
protected:
    void addFolderByFile(const QString &fileInFolderPath);
    static inline bool isFolder(const QString &path);
    static inline QString parentFolder(const QString &path);
    QString findFilePathByRefEnd(
            const QString &currentFolderPath,
            const QString &searchingForEnd,
            QStringList &folderBlackList) const;
    QHash<QString, QStringList> m_folderToFilePathesHash;
};

#endif // LINKER_H
