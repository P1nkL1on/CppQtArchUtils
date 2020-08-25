#ifndef FILE_LINK_H
#define FILE_LINK_H

#include <QString>

class File;

class FileLink
{
public:
    FileLink() = default;
    FileLink(const QString &includePath);
    QString includePath() const;
    bool isResolved() const;
    File *filePtr() const;
    bool tryResolve(File *includer);

    static File *tryResolve(
            const QString &includePath,
            File *includer);

protected:
    QString m_includePath;
    File *m_filePtr = nullptr;
};

#endif // FILE_LINK_H
