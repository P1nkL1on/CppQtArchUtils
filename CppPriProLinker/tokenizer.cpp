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
    int currentPos = 0;
    QVector<Token> res;
    QVector<int> cacheTokenPos(tokenRegExpPatterns.size(), -1);
    while (currentPos < plainData.size()){
        int nextInd= -1;
        int minNextPos = plainData.size();
        for (int typeInd = 0; typeInd < tokenRegExpPatterns.size(); ++typeInd){
            const QRegExp &reg = tokenRegExpPatterns[typeInd];
            int nextPos = cacheTokenPos[typeInd];
            if (nextPos < currentPos){
                nextPos = reg.indexIn(plainData, currentPos);
                cacheTokenPos[typeInd] = nextPos;
            }
            if (nextPos >= currentPos and nextPos < minNextPos){
                minNextPos = nextPos;
                nextInd = typeInd;
            }
            if (nextPos == currentPos)
                break;
        }
        if (nextInd == -1)
            break;
        const int length = tokenRegExpPatterns[int(nextInd)].matchedLength();
        const QString tokenText = plainData.mid(minNextPos, length);
        res << Token(minNextPos, nextInd, tokenText);
        currentPos = minNextPos + length;
    }
    return res;
}

Tokenizer Tokenizer::headerTokenizer()
{
    const QMap<int, QString> regExpMap {
        {int(TokenType::Include),     "#include((\\s*)((<([^>]*)>|\"([^\"]*)\")))"},
        {int(TokenType::Directive),   "#(\\S*)(()|([^\\s+]*))"},
        {int(TokenType::AreaComment), "\\/\\*([^*]|\\*(?!\\/))*\\*\\/"},
        {int(TokenType::Char),        "\'(\\\\?[^\'\\n]|\\\\\')\'?"},
        {int(TokenType::Qoute),       "\"([^\"\\n]|\\\\\")*\"?"},
        {int(TokenType::LineComment), "\\/\\/[^\n]*"},
        {int(TokenType::CurlyOpen),   "\\{"},
        {int(TokenType::CurlyClose),  "\\}"},
        {int(TokenType::SemiColon),   ";"},
        {int(TokenType::Identifer),   "([a-zA-Z_][a-zA-Z_:0-9]*)"}
    };
    return regExpMapToTokenizer(regExpMap);
}

Tokenizer Tokenizer::priTokenizer()
{
    const QMap<int, QString> regExpMap {
        {int(TokenType::LineComment), "#[^\n]*"},
        {int(TokenType::CurlyOpen),   "\\{"},
        {int(TokenType::CurlyClose),  "\\}"}
    };
    return regExpMapToTokenizer(regExpMap);
}

Tokenizer Tokenizer::regExpMapToTokenizer(const QMap<int, QString> &regExpMap)
{
    QVector<QRegExp> tokenRegExpPatterns;
    for (const QString &pattern : regExpMap)
        tokenRegExpPatterns << QRegExp(pattern);

    return Tokenizer(tokenRegExpPatterns);
}
