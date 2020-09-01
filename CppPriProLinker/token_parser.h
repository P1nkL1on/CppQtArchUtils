#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "ref_class.h"
#include "ref_file.h"
#include "token.h"

class ProConfig;

class TokenParser
{
public:
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
            const QVector<Token> &tokens,
            QVector<RefFile> &links,
            QVector<ProConfig> &configs);
};

#endif // FILEPARSER_H
