#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "file_data.h"
#include "ref_class.h"
#include "ref_file.h"
#include "token.h"

class ProConfig;

class TokenParser
{
public:
    static bool readPlainFileData(
            const QString &filePath,
            PlainFileData &data,
            QString &errMessage);

    static void parseCpp(
            const QVector<Token> &tokens,
            QVector<RefFile> &includes,
            QVector<RefClass> &classes,
            QString &guard);

    static RefFile includeTokenToRawRef(
            const Token &token);
    static RefClass classTokenToRawRef(const QStringList &currentBlockStack,
            const Token &token);
    static RefClass tokenToRawRef(
            const QStringList &currentBlockStack,
            const Token &token);
    static void parsePro(
            const FileData &data,
            QVector<RefFile> &links);
};

#endif // FILEPARSER_H
