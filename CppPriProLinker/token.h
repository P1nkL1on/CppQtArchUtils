#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

enum class CppTokenType
{
    None = -1,
    AreaComment = 0,
    LineComment,
    Qoute,
    Char,
    Include,
    Directive,
    CurlyOpen,
    CurlyClose,
    SemiColon,
    Identifer,
};

enum class ProTokenType
{
    None = -1,
    Comment = 0,
    Case,
    CurlyOpen,
    CurlyClose,
    Operator,
    LineContinue,
    LineBreak,
    Identifer,
};

struct Token
{
    Token() = default;
    Token(int pos, int type, const QString &text) :
        pos(pos), type(type), text(text) {}
    int pos = -1;
    int type = -1;
    QString text;
};

#endif // TOKEN_H
