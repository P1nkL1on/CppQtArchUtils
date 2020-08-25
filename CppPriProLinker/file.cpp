#include "file.h"

File::File(const QString &filePath) :
    m_fileInfo(filePath)
{
}

const QFileInfo &File::info() const
{
    return m_fileInfo;
}

const QVector<FileLink> &File::links() const
{
    return m_links;
}
