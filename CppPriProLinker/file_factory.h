#ifndef FILE_FACTORY_H
#define FILE_FACTORY_H

#include "file.h"

class FileFactory
{
public:
    FileFactory() = default;
    virtual ~FileFactory() = default;
    virtual File *read(const QString &filePath, QString &err) const = 0;
};

#endif // FILE_FACTORY_H
