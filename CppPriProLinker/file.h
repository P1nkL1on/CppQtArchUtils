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
    virtual QVector<RefFile> refFiles() const;
    void addRefToFile(const RefFile &ref);
protected:
    QFileInfo m_fileInfo;
    QVector<RefFile> m_refFiles;
};

#endif // FILE_H
