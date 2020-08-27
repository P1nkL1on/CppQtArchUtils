#ifndef FILE_TOKENIZER_H
#define FILE_TOKENIZER_H

#include <QVector>

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

class FileTokenizer
{
public:
    // todo create factory for cpp / pri
    FileTokenizer();
    QVector<Token> tokenize(const QString &text) const;
    // todo move to parser
    void parseCpp(
            const QString &text,
            QStringList &includes,
            QStringList &namespaces,
            QStringList &classes,
            QString &guard) const;
    static QVector<Token> tokenize(
            const QString &text,
            const QVector<QRegExp> &tokenRegExpPatterns,
            const QVector<TokenType> &dismissTokenTypes = {});
protected:
    QVector<QRegExp> m_tokenRegExpPatterns;

};

#endif // FILE_TOKENIZER_H
