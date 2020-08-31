#include "file.h"

void File::addRawRefs(const QVector<RefFile> &rawRefs)
{
    for (const RefFile &r : rawRefs)
        setRef(r, nullptr);
}

void File::setRef(const RefFile &ref, File *file)
{
    m_refToFileHash.insert(ref, file);
}

QList<RefFile> File::refs() const
{
    return m_refToFileHash.keys();
}
