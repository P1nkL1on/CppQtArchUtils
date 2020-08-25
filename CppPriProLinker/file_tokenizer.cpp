#include "file_tokenizer.h"

#include <QRegExp>
#include <QMap>
#include <QtDebug>

FileTokenizer::FileTokenizer()
{
    QMap<TokenType, QString> tokenTypeToRegExpPattern {
        {StructBlockOpen,    "(class|struct|namespace)"
                      "(\\s+)([a-zA-Z_][a-zA-Z_:0-9]*)"
                      "(\\s*)(:[^\\{]*)?\\{"},
        {Directive,   "#(\\S*)(\\s+)((<([^>]*)>|"
                      "\"([^\"]*)\")|([^\\s+]*))"},
        {AreaComment, "\\/\\*([^*]|\\*(?!\\/))*\\*\\/"},
        {Char,        "\'(\\\\?[^\'\\n]|\\\\\')\'?"},
        {Qoute,       "\"([^\"\\n]|\\\\\")*\"?"},
        {LineComment, "\\/\\/[^\n]*"},
        {BlockOpen,   "\\{"},
        {BlockClose,  "\\}"}
    };

    m_skipRegExpPattern = QRegExp(
                "(class|struct|namespace)|"
                "(\\/\\/)|(\\/\\*)|#|\"|\'|\\{|\\}");

    for (const QString &pattern : tokenTypeToRegExpPattern)
        m_tokenRegExpPatterns << QRegExp(pattern);
}

QVector<Token> FileTokenizer::tokenize(const QString &text) const
{
    return tokenize(text, m_tokenRegExpPatterns, m_skipRegExpPattern);
}

void FileTokenizer::parse(const QString &text, QStringList &includes, QStringList &namespaces, QStringList &classes, QString &guard) const
{
    const QVector<Token> tokens = tokenize(text, m_tokenRegExpPatterns, m_skipRegExpPattern,
        {AreaComment, LineComment, Qoute, Char, BlockOpen, BlockClose});
    const QRegExp spaceRegExp("(\\s|\\{)+");
    for (const Token &token : tokens){
        const QStringList splitedToken = token.text.trimmed()
                .split(spaceRegExp, QString::SkipEmptyParts);
        const QString value = splitedToken[1];
        if (token.type == Directive){
            if (splitedToken.first() == "#include")
                includes << value;
            else if (guard.isEmpty() and splitedToken.first() == "#ifndef")
                guard = value;
        } else if (token.type == StructBlockOpen){
            if (splitedToken.first() == "namespace")
                namespaces << value;
            else
                classes << value;
        }
    }
}

QVector<Token> FileTokenizer::tokenize(const QString &text,
        const QVector<QRegExp> &tokenRegExpPatterns,
        const QRegExp &skipRegExpPattern,
        const QVector<TokenType> &dismissTokenTypes)
{
    int currentInd = 0;
    QVector<Token> res;
    QVector<int> cacheTokenPos(tokenRegExpPatterns.size(), -1);
    while (currentInd < text.size()){
        int capturedInd = -1;
        TokenType foundType = None;
        for (int typeInd = 0; typeInd < 8; ++typeInd){
            const TokenType tokenType = TokenType(typeInd);
            const QRegExp &reg = tokenRegExpPatterns[tokenType];
            capturedInd = cacheTokenPos[tokenType];
            if (capturedInd < currentInd){
                capturedInd = reg.indexIn(text, currentInd);
                cacheTokenPos[tokenType] = capturedInd;
            }
            if (capturedInd == currentInd){
                const int length = reg.matchedLength();
                if (not dismissTokenTypes.contains(tokenType)){
                    const QString tokenText = text.mid(currentInd, length);
                    res << Token(tokenType, tokenText);
                }
                foundType = tokenType;
                currentInd += length;
                break;
            }
        }
        if (foundType == None){
            const int jumpToInd = text.indexOf(skipRegExpPattern, currentInd + 1);
            currentInd = jumpToInd >= 0 ? jumpToInd : (text.size());
        }
    }
    return res;
}
