#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "file_data.h"
#include "file_link.h"
#include "token.h"

class TokenParser
{
public:
    static bool readFileData(
            const QString &filePath,
            FileData &data,
            QString &errMessage);

    static void parseCpp(const QVector<Token> &tokens,
            QStringList &includes,
            QStringList &classes,
            QString &guard);

    static void parsePri(
            const FileData &data,
            QVector<FileLink> &links);
};

#endif // FILEPARSER_H
