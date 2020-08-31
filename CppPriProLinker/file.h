#ifndef FILE_H
#define FILE_H

#include "ref_file.h"

#include <QFileInfo>
#include <QVector>

class Folder;

class File
{
public:
    explicit File() = default;
    virtual ~File() = default;
    void addRawRefs(const QVector<RefFile> &rawRefs);
    void setRef(const RefFile &ref, File *file);
    QList<RefFile> refs() const;
protected:
    QHash<RefFile, File *> m_refToFileHash;
};

#endif // FILE_H
