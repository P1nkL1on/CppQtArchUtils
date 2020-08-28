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
    static Tokenizer cppTokenizer();
    static Tokenizer proTokenizer();
protected:
    static Tokenizer regExpMapToTokenizer(
            const QMap<int, QString> &regExpMap);
    QVector<QRegExp> m_tokenRegExpPatterns;

};

#endif // FILE_TOKENIZER_H
