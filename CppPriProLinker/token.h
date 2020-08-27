#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

enum TokenType
{
    None = -1,
    AreaComment = 0,
    LineComment,
    Qoute,
    Char,
    Directive,
    BlockOpen,
    BlockClose,
    StructBlockOpen
};

struct Token
{
    Token() = default;
    Token(TokenType type, const QString &text) :
        type(type), text(text) {}
    TokenType type = None;
    QString text;
};

#endif // TOKEN_H
