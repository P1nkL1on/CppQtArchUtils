#include "linker.h"

#include <QDir>

void Linker::addFilePathes(const QStringList &absFilePathes)
{
    for (const QString &filePath : absFilePathes)
        addFolderByFile(filePath);
}

QString Linker::findFilePathForRef(
        const QString &origFilePath,
        const QString &refText) const
{
    const QString folderPath = parentFolder(origFilePath);
    const FileType type = fileType(origFilePath);
    if (isCodeFormate(type)){
        QString refTextWithSlash = refText;
        if (not refTextWithSlash.startsWith('/'))
            refTextWithSlash.insert(0, '/');
        QStringList blackList;
        const QString res = findFilePathByRefEnd(folderPath, refTextWithSlash, blackList);
        return res;
    } else if (isProjectFormat(type)){
        QString absPath = refText;
        absPath.replace("$$PWD", "");
        if (not absPath.startsWith('/'))
            absPath.insert(0, '/');
        absPath = folderPath + absPath;
        const QString res = QDir::cleanPath(absPath);
        return res;
    } else
        return QString();
}

QString Linker::findFilePathForRef(const QString &origFilePath, const RefFile &ref) const
{
    if (ref.isSystem)
        return QString();
    return findFilePathForRef(origFilePath, ref.text);
}

bool Linker::isFolder(const QString &path)
{
    return path.contains("/");
}

QString Linker::parentFolder(const QString &path)
{
    return path.mid(0, path.lastIndexOf("/"));
}

void Linker::addFolderByFile(const QString &fileInFolderPath)
{
    if (not isFolder(fileInFolderPath))
        return;
    const QString folderPath = parentFolder(fileInFolderPath);
    if (not m_folderToFilePathesHash.contains(folderPath))
        addFolderByFile(folderPath);
    m_folderToFilePathesHash[folderPath] << fileInFolderPath;
}

QString Linker::findFilePathByRefEnd(
        const QString &currentFolderPath,
        const QString &searchingForEnd,
        QStringList &folderBlackList) const
{
    const QStringList filePathesInFolder = m_folderToFilePathesHash.value(currentFolderPath);
    QStringList foldersToCheck {parentFolder(currentFolderPath)};
    for (const QString &p : filePathesInFolder){
        if (m_folderToFilePathesHash.contains(p))
            foldersToCheck << p;
        else if (p.endsWith(searchingForEnd))
            return p;
    }
    folderBlackList << currentFolderPath;
    for (const QString &f : foldersToCheck)
        if (not folderBlackList.contains(f)){
            const QString res = findFilePathByRefEnd(f, searchingForEnd, folderBlackList);
            if (not res.isEmpty())
                return res;
        }
    return QString();
}
