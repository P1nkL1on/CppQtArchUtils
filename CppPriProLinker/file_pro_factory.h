#ifndef FILEPROFACTORY_H
#define FILEPROFACTORY_H

#include "file_factory.h"

class FileProFactory : public FileFactory
{
public:
    FileProFactory() = default;
protected:
    Tokenizer tokenizer() const override;
    File * createFile(
            const QFileInfo &info,
            const QVector<Token> &tokens) const override;
};


#endif // FILEPROFACTORY_H
