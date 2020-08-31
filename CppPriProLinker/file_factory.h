#ifndef FILE_FACTORY_H
#define FILE_FACTORY_H

#include "file.h"
#include "tokenizer.h"

class FileFactory
{
public:
    FileFactory() = default;
    virtual ~FileFactory() = default;
    File *read(const QString &filePath, QString &err) const;
protected:
    virtual Tokenizer tokenizer() const = 0;
    virtual File *createFile(
            const QFileInfo &info,
            const QVector<Token> &tokens) const = 0;
};

#endif // FILE_FACTORY_H
