#ifndef FILEPROFACTORY_H
#define FILEPROFACTORY_H

#include "file_factory.h"
#include "tokenizer.h"

class FileProFactory : public FileFactory
{
public:
    FileProFactory() = default;
    File *read(const QString &filePath, QString &err) override;
protected:
    Tokenizer m_tokenizer = Tokenizer::proTokenizer();
};


#endif // FILEPROFACTORY_H
