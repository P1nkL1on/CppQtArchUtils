#include "tokenizer.h"

#include <QRegExp>
#include <QMap>
#include <QtDebug>

Tokenizer::Tokenizer(const QVector<QRegExp> &tokenRegExpPatterns) :
    m_tokenRegExpPatterns(tokenRegExpPatterns)
{
}

QVector<Token> Tokenizer::tokenize(const PlainFileData &plainData) const
{
    return tokenize(plainData, m_tokenRegExpPatterns);
}

QVector<Token> Tokenizer::tokenize
(const PlainFileData &plainData,
            const QVector<QRegExp> &tokenRegExpPatterns)
{
    int currentInd = 0;
    QVector<Token> res;
    QVector<int> cacheTokenPos(tokenRegExpPatterns.size(), -1);
    while (currentInd < plainData.size()){
        TokenType nextType = None;
        int minNextInd = plainData.size();
        for (int typeInd = 0; typeInd < tokenRegExpPatterns.size(); ++typeInd){
            const TokenType tokenType = TokenType(typeInd);
            const QRegExp &reg = tokenRegExpPatterns[tokenType];
            int nextInd = cacheTokenPos[tokenType];
            if (nextInd < currentInd){
                nextInd = reg.indexIn(plainData, currentInd);
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
        const QString tokenText = plainData.mid(minNextInd, length);
        res << Token(nextType, tokenText);
        currentInd = minNextInd + length;
    }
    return res;
}

Tokenizer Tokenizer::headerTokenizer()
{
    // todo
    // before tokenizing remove comments, qoutes and chars in text
    const QMap<TokenType, QString> regExpMap {
        {StructBlockOpen,    "((namespace(\\s+)([a-zA-Z_][a-zA-Z_:0-9]*)(\\s*)\\{))|" // namespace A::B {
                             "((class|struct)(\\s+)([a-zA-Z_][a-zA-Z_:0-9]*)"         // class_%name%
                             "(\\s*)(:[^\\{]*)?\\{)"},                                // : public %base_class_name% {
        {Directive,   "#(\\S*)((\\s*)((<([^>]*)>|\"([^\"]*)\"))|"   // #dir <%something>, #dir "okey"
                      "()|([^\\s+]*))"},                            // #dir Any Other Text, #endif
        {AreaComment, "\\/\\*([^*]|\\*(?!\\/))*\\*\\/"},
        {Char,        "\'(\\\\?[^\'\\n]|\\\\\')\'?"},
        {Qoute,       "\"([^\"\\n]|\\\\\")*\"?"},
        {LineComment, "\\/\\/[^\n]*"},
        {BlockOpen,   "\\{"},
        {BlockClose,  "\\}"}
    };
    return regExpMapToTokenizer(regExpMap);
}

Tokenizer Tokenizer::priTokenizer()
{
    const QMap<TokenType, QString> regExpMap {
        {LineComment, "#[^\n]*"},
        {BlockOpen,   "\\{"},
        {BlockClose,  "\\}"}
    };
    return regExpMapToTokenizer(regExpMap);
}

Tokenizer Tokenizer::regExpMapToTokenizer(const QMap<TokenType, QString> &regExpMap)
{
    QVector<QRegExp> tokenRegExpPatterns;
    for (const QString &pattern : regExpMap)
        tokenRegExpPatterns << QRegExp(pattern);

    return Tokenizer(tokenRegExpPatterns);
}
