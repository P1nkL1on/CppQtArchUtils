#ifndef FILE_H
#define FILE_H

#include "file_link.h"

#include <QFileInfo>
#include <QVector>

class File
{
public:
    File(const QString &filePath);
    const QFileInfo &info() const;
    const QVector<FileLink> &links() const;
protected:
    QFileInfo m_fileInfo;
    QVector<FileLink> m_links;
};

#endif // FILE_H
