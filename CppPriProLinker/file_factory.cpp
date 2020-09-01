#include "file_factory.h"

#include "file_data.h"
#include "token.h"
#include "file_io.h"

File *FileFactory::parse(
        const QString &filePath,
        QString &err) const
{
    PlainFileData data;
    if (not FileIO::readPlainFileData(filePath, data, err))
        return nullptr;
    const QVector<Token> tokens = tokenizer().tokenize(data);
    return parseFileFromTokens(tokens);
}
