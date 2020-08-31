#include "file_factory.h"

#include "file_data.h"
#include "token.h"
#include "token_parser.h"

File *FileFactory::read(
        const QString &filePath,
        QString &err) const
{
    PlainFileData data;
    if (not TokenParser::readPlainFileData(filePath, data, err))
        return nullptr;
    const QVector<Token> tokens = tokenizer().tokenize(data);
    return createFile(tokens);
}
