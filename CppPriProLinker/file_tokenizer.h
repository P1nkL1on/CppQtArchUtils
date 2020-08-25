#ifndef FILE_TOKENIZER_H
#define FILE_TOKENIZER_H

#include <QVector>

enum TokenType
{
    None = -1,
    Comment = 0,
    Qoute,
    Char,
    Directive,
    OpenCurly,
    CloseCurly,
    KeyWord,
    Name,
    Num,
    WhiteSpace
};

struct Token
{
    Token() = default;
    Token(TokenType type, const QString &text) :
        type(type), text(text) {}
    TokenType type = None;
    QString text;
};

class FileTokenizer
{
public:
    static QVector<Token> tokenize(const QString &text);

};

#endif // FILE_TOKENIZER_H
