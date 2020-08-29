#ifndef FILECPPFACTORY_H
#define FILECPPFACTORY_H

#include "file_factory.h"
#include "tokenizer.h"

class FileCppFactory : public FileFactory
{
public:
    FileCppFactory() = default;
    File *read(const QString &filePath, QString &err) const override;
protected:
    Tokenizer m_tokenizer = Tokenizer::cppTokenizer();
};

#endif // FILECPPFACTORY_H
