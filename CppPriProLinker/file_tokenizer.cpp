#include "file_tokenizer.h"

#include <QRegExp>
#include <QMap>
#include <QtDebug>

FileTokenizer::FileTokenizer()
{
    QMap<TokenType, QString> tokenTypeToRegExpPattern {
        {StructBlockOpen,    "((namespace(\\s+)([a-zA-Z_][a-zA-Z_:0-9]*)(\\s*)\\{))|" // namespace A::B {
                             "((class|struct)(\\s+)([a-zA-Z_][a-zA-Z_:0-9]*)" // class_%name%
                             "(\\s*)(:[^\\{]*)?\\{)"},      // : public %base_class_name% {
        {Directive,   "#(\\S*)(\\s+)((<([^>]*)>|"
                      "\"([^\"]*)\")|([^\\s+]*))"},
        {AreaComment, "\\/\\*([^*]|\\*(?!\\/))*\\*\\/"},
        {Char,        "\'(\\\\?[^\'\\n]|\\\\\')\'?"},
        {Qoute,       "\"([^\"\\n]|\\\\\")*\"?"},
        {LineComment, "\\/\\/[^\n]*"},
        {BlockOpen,   "\\{"},
        {BlockClose,  "\\}"}
    };
    for (const QString &pattern : tokenTypeToRegExpPattern)
        m_tokenRegExpPatterns << QRegExp(pattern);
}

QVector<Token> FileTokenizer::tokenize(const QString &text) const
{
    return tokenize(text, m_tokenRegExpPatterns);
}

void FileTokenizer::parseCpp(
            const QString &text,
            QStringList &includes,
            QStringList &namespaces,
            QStringList &classes,
            QString &guard) const
{
    const QVector<Token> tokens = tokenize(text, m_tokenRegExpPatterns,
        {AreaComment, LineComment, Qoute, Char});
    const QRegExp spaceRegExp("(\\s|\\{)+");
    QStringList currentNamespaces;
    int currentBlockDepth = 0;
    for (const Token &token : tokens){
        if (token.type == BlockOpen){
            ++currentBlockDepth;
            continue;
        }
        if (token.type == BlockClose){
            if (currentBlockDepth < currentNamespaces.length())
                currentNamespaces.removeLast();
            --currentBlockDepth;
            continue;
        }
        const QStringList splitedToken = token.text.trimmed()
                .split(spaceRegExp, QString::SkipEmptyParts);
        const QString value = splitedToken.size() > 1 ?
                    splitedToken[1] : QString();
        if (token.type == Directive){
            if (splitedToken.first() == "#include")
                includes << value;
            else if (guard.isEmpty() and splitedToken.first() == "#ifndef")
                guard = value;
        } else if (token.type == StructBlockOpen){
            currentNamespaces << value;
            ++currentBlockDepth;
            if (not namespaces.contains(value) and splitedToken.first() == "namespace"){
                namespaces << value;
            } else {
                classes << currentNamespaces.join("::");
            }
        }
    }
}

QVector<Token> FileTokenizer::tokenize(
            const QString &text,
            const QVector<QRegExp> &tokenRegExpPatterns,
            const QVector<TokenType> &dismissTokenTypes)
{
    int currentInd = 0;
    QVector<Token> res;
    QVector<int> cacheTokenPos(8, -1);
    while (currentInd < text.size()){
        TokenType nextType = None;
        int minNextInd = text.size();
        for (int typeInd = 0; typeInd < tokenRegExpPatterns.size(); ++typeInd){
            const TokenType tokenType = TokenType(typeInd);
            const QRegExp &reg = tokenRegExpPatterns[tokenType];
            int nextInd = cacheTokenPos[tokenType];
            if (nextInd < currentInd){
                nextInd = reg.indexIn(text, currentInd);
                cacheTokenPos[tokenType] = nextInd;
            }
            if (nextInd >= currentInd and nextInd < minNextInd){
                minNextInd = nextInd;
                nextType = tokenType;
            }
            if (nextInd == currentInd)
                break;
        }
        if (nextType == None)
            break;
        const int length = tokenRegExpPatterns[nextType].matchedLength();
        if (not dismissTokenTypes.contains(nextType)){
            const QString tokenText = text.mid(minNextInd, length);
            res << Token(nextType, tokenText);
        }
        currentInd = minNextInd + length;
    }
    return res;
}
