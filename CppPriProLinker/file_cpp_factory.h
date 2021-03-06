#ifndef FILECPPFACTORY_H
#define FILECPPFACTORY_H

#include "file_factory.h"

class FileCppFactory : public FileFactory
{
public:
    FileCppFactory() = default;
protected:
    Tokenizer tokenizer() const override;
    File *parseFileFromTokens(const QVector<Token> &tokens) const override;
};

#endif // FILECPPFACTORY_H
