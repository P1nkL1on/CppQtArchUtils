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
        {BlockClose,  "\\}"},
        {Skip,        "(class|struct|namespace)|(\\/\\/)|(\\/\\*)|#|\"|\'|\\{|\\}"}
    };
    for (const QString &pattern : tokenTypeToRegExpPattern)
        m_tokenRegExpPatterns << QRegExp(pattern);
}

QVector<Token> FileTokenizer::tokenize(const QString &text) const
{
    return tokenize(text, m_tokenRegExpPatterns);
}

void FileTokenizer::parse(
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
        const QString value = splitedToken[1];
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
    Q_ASSERT(tokenRegExpPatterns.size() == 9);
    int currentInd = 0;
    QVector<Token> res;
    const QRegExp &skipRegExpPattern = tokenRegExpPatterns[Skip];
    QVector<int> cacheTokenPos(8, -1);
    while (currentInd < text.size()){
        int capturedInd = -1;
        TokenType foundType = None;
        for (int typeInd = AreaComment; typeInd <= StructBlockOpen; ++typeInd){
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
