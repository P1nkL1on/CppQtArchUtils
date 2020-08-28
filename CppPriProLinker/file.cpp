#include "file.h"

File::File(const QFileInfo &fileInfo) :
    m_fileInfo(fileInfo)
{
}

File::~File()
{
}

QVector<RefFile> File::refFiles() const
{
    return m_refFiles;
}

void File::addRefToFile(const RefFile &ref)
{
    if (not m_refFiles.contains(ref))
        m_refFiles << ref;
}
