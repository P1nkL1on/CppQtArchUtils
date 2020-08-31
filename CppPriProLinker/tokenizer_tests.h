#ifndef TOKENIZER_TESTS_H
#define TOKENIZER_TESTS_H

#include <QObject>

#include "tokenizer.h"

class TokenizerTests : public QObject
{
    Q_OBJECT
public:
    TokenizerTests(QObject *parent = nullptr);
    ~TokenizerTests() override;
private slots:
    void headerEmptyData();
    void headerBracketsAndOtherCode();
    void headerCommentsMasking();
    void compareProTokens();
protected:
    struct HeaderTokenizer
    {
        enum TpkenizerType {Cpp, Pro};
        HeaderTokenizer(TpkenizerType type = Cpp);
        ~HeaderTokenizer();
        void tokenizeAndCache(
            const PlainFileData &data);
        Tokenizer tokenizer;
        QVector<Token> cachedTokens;
        QVector<int> cachedTypes;
        QStringList cachedTexts;
        QString cachedString;
    };
};

#endif // TOKENIZER_TESTS_H
