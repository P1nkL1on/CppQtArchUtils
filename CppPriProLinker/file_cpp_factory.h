#ifndef FILECPPFACTORY_H
#define FILECPPFACTORY_H

#include "file_factory.h"

class FileCppFactory : public FileFactory
{
public:
    FileCppFactory() = default;
    File *read(const QString &filePath, QString &err) override;
};

#endif // FILECPPFACTORY_H
