#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "file_data.h"
#include "file_link.h"
#include "token.h"


class TokenParser
{
public:
    static bool readPlainFileData(
            const QString &filePath,
            PlainFileData &data,
            QString &errMessage);

    static void parseCpp(const QVector<Token> &tokens,
            QStringList &includes,
            QStringList &classes,
            QString &guard);

    static void parsePro(
            const FileData &data,
            QVector<FileLink> &links);
};

#endif // FILEPARSER_H
