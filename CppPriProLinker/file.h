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
protected:
    QHash<RefFile, File *> m_refs;
};

#endif // FILE_H
