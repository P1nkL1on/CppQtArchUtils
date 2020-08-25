#include "file_tokenizer.h"

#include <QRegExp>
#include <QMap>
#include <QtDebug>

QVector<Token> FileTokenizer::tokenize(const QString &text)
{
    const QMap<TokenType, QRegExp> regs {
        {AreaComment, QRegExp("\\/\\*([^*]|\\*(?!\\/))*\\*\\/")},
        {LineComment, QRegExp("\\/\\/[^\n]*")},
        {Qoute,       QRegExp("\"([^\"\\n]|\\\\\")*\"?")},
        {Char,        QRegExp("\'(\\\\?[^\'\\n]|\\\\\')\'?")},
        {Directive,   QRegExp("#(\\S*)(\\s+)((<([^>]*)>|"
                              "\"([^\"]*)\")|([^\\s+]*))")},
        {OpenCurly,   QRegExp("\\{")},
        {CloseCurly,  QRegExp("\\}")},
        {Block,       QRegExp("(class|struct|namespace)"
                              "(\\s+)([a-zA-Z_$][a-zA-Z_$0-9]*)"
                              "(\\s*)(:[^\\{]*)?\\{")},
    };

    int currentInd = 0;

    QVector<Token> res;
    QMap<TokenType, int> cacheTokenPos;

    while (currentInd < text.size()){
        int capturedInd = -1;
        TokenType foundType = None;
        for (const QRegExp &reg : regs){
            const TokenType tokenType = regs.key(reg);
            capturedInd = cacheTokenPos.value(tokenType, -1);
            if (capturedInd < currentInd){
                capturedInd = reg.indexIn(text, currentInd);
                cacheTokenPos.insert(tokenType, capturedInd);
            }
            if (capturedInd == currentInd){
                foundType = tokenType;
                const int length = reg.matchedLength();
                const QString tokenText = text.mid(currentInd, length);
                currentInd += length;
                res << Token(foundType, tokenText);
                break;
            }
        }
        if (foundType == None){
            const int jumpToInd = text.indexOf(QRegExp("(class|struct|namespace)|(\\/\\/)|(\\/\\*)|#|\"|\'|\\{|\\}"), currentInd + 1);
            currentInd = jumpToInd >= 0 ? jumpToInd : (text.size());
        }
    }
    return res;
}
