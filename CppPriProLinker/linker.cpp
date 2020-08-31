#include "linker.h"

#include <QDir>

Linker::Linker(const QStringList &absFilePathes)
{
    for (const QString &filePath : absFilePathes)
        addFolderByFile(filePath);
}

QString Linker::findFilePathForRef(
        const QString &filePath,
        const QString &refText,
        Linker::RefType refType) const
{
    const QString folderPath = parentFolder(filePath);
    if (refType == RefType::Cpp){
        QString refTextWithSlash = refText;
        if (not refTextWithSlash.startsWith('/'))
            refTextWithSlash.insert(0, '/');
        QStringList blackList;
        return findFilePathByRefEnd(folderPath, refTextWithSlash, blackList);
    } else if (refType == RefType::Pro){
        QString absPath = refText;
        absPath.replace("$$PWD", "");
        absPath = folderPath + absPath;
        return QDir::cleanPath(absPath);
    } else
        return QString();
}

bool Linker::isFolder(const QString &path) const
{
    return path.contains("/");
}

QString Linker::parentFolder(const QString &path) const
{
    return path.mid(0, path.lastIndexOf("/"));
}

void Linker::addFolderByFile(const QString &fileInFolderPath)
{
    if (not isFolder(fileInFolderPath))
        return;
    const QString folderPath = parentFolder(fileInFolderPath);
    if (not folderToFilesPathesHash.contains(folderPath))
        addFolderByFile(folderPath);
    folderToFilesPathesHash[folderPath] << fileInFolderPath;
}

QString Linker::findFilePathByRefEnd(
        const QString &currentFolderPath,
        const QString &searchingForEnd,
        QStringList &folderBlackList) const
{
    const QStringList filePathesInFolder = folderToFilesPathesHash.value(currentFolderPath);
    QStringList foldersToCheck {parentFolder(currentFolderPath)};
    for (const QString &p : filePathesInFolder){
        if (folderToFilesPathesHash.contains(p))
            foldersToCheck << p;
        else if (p.endsWith(searchingForEnd))
            return p;
    }
    folderBlackList << currentFolderPath;
    for (const QString &f : foldersToCheck)
        if (not folderBlackList.contains(f)){
            const QString res = findFilePathByRefEnd(
                        f, searchingForEnd, folderBlackList);
            if (not res.isEmpty())
                return res;
        }
    return QString();
}
