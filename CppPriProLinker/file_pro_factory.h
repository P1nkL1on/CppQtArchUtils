#ifndef FILEPROFACTORY_H
#define FILEPROFACTORY_H

#include "file_factory.h"

class FileProFactory : public FileFactory
{
public:
    FileProFactory() = default;
    File *read(const QString &filePath, QString &err) override;
};


#endif // FILEPROFACTORY_H
