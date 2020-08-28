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
protected:
    struct HeaderTokenizer
    {
        HeaderTokenizer();
        ~HeaderTokenizer();
        void tokenizeAndCache(
            const PlainFileData &data);
        Tokenizer tokenizer;
        QVector<Token> cachedTokens;
        QVector<int> cachedTypes;
        QStringList cachedTexts;
    };
};

#endif // TOKENIZER_TESTS_H
