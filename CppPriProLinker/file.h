#ifndef FILE_H
#define FILE_H

#include "ref_file.h"

#include <QFileInfo>
#include <QVector>

class File
{
public:
    explicit File(const QFileInfo &fileInfo);
    virtual ~File();
    QHash<RefFile, File *> refs;
protected:
    QFileInfo m_fileInfo;
};

#endif // FILE_H
