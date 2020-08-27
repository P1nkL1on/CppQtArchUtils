#ifndef FILE_TOKENIZER_H
#define FILE_TOKENIZER_H

#include <QVector>

#include "token.h"
#include "file_data.h"

class Tokenizer
{
public:
    Tokenizer(
            const QVector<QRegExp> &tokenRegExpPatterns);
    QVector<Token> tokenize(
            const PlainFileData &plainData) const;
    static QVector<Token> tokenize(
            const PlainFileData &plainData,
            const QVector<QRegExp> &tokenRegExpPatterns);
    static Tokenizer headerTokenizer();
    static Tokenizer priTokenizer();
protected:
    static Tokenizer regExpMapToTokenizer(
            const QMap<TokenType, QString> &regExpMap);
    QVector<QRegExp> m_tokenRegExpPatterns;

};

#endif // FILE_TOKENIZER_H
