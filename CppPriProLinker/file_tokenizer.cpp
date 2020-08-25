#include "file_tokenizer.h"

#include <QRegExp>
#include <QMap>
#include <QtDebug>

QVector<Token> FileTokenizer::tokenize(const QString &text)
{
    QMap<TokenType, QRegExp> regs {
        {AreaComment, QRegExp("\\/\\*([^*]|\\*(?!\\/))*\\*\\/")},
        {LineComment, QRegExp("\\/\\/[^\n]*")},
        {Directive, QRegExp("#(\\S*)(\\s+)((<([^>]*)>|\"([^\"]*)\")|([^\\s+]*))")},
        {Qoute, QRegExp("\"([^\"\\n]|\\\\\")*\"?")},
        {Char, QRegExp("\'(\\\\?[^\'\\n]|\\\\\')\'?")},
        {OpenCurly, QRegExp("\\{")},
        {CloseCurly, QRegExp("\\}")},
        {Block, QRegExp("(class|struct|namespace)(\\s+)([a-zA-Z_$][a-zA-Z_$0-9]*)(\\s*)\\{")},
    };

    int currentInd = 0;

    QVector<Token> res;
    while (currentInd < text.size()){
        int capturedInd = -1;
        TokenType foundType = None;
        for (const QRegExp &reg : regs){
            capturedInd = reg.indexIn(text, currentInd);
            if (capturedInd == currentInd){
                foundType = regs.key(reg);
                const int length = reg.matchedLength();
                const QString tokenText = text.mid(currentInd, length);
                currentInd += length;
                res << Token(foundType, tokenText);
                break;
            }
        }
        if (foundType == None)
//            break;
            ++currentInd;
    }
    return res;
}
