#include "file_link.h"

#include "file.h"

FileLink::FileLink(const QString &includePath) :
    m_includePath(includePath)
{
}

bool FileLink::isResolved() const
{
    return m_filePtr;
}

File *FileLink::filePtr() const
{
    return m_filePtr;
}

bool FileLink::tryResolve(File *includer)
{
    m_filePtr = tryResolve(m_includePath, includer);
    return isResolved();
}

File *FileLink::tryResolve(const QString &includePath, File *includer)
{
    return nullptr;
}

QString FileLink::includePath() const
{
    return m_includePath;
}
