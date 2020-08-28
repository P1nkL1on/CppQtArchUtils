#include "tokenizer_tests.h"
#include <QtTest>

TokenizerTests::TokenizerTests(QObject *parent) :
    QObject(parent)
{
}

TokenizerTests::~TokenizerTests()
{
}

void TokenizerTests::headerEmptyData()
{
    HeaderTokenizer t;
    t.tokenizeAndCache(QString());
    QVERIFY(t.cachedTokens.isEmpty());
    QVERIFY(t.cachedTypes.isEmpty());
    QVERIFY(t.cachedTexts.isEmpty());
}

void TokenizerTests::headerBracketsAndOtherCode()
{
    HeaderTokenizer t;
    t.tokenizeAndCache("a {b} { c } {{ d }} { { e } f} {{{}}}");
    const QStringList expectedTokenTexts {
        "{", "}", "{", "}", "{", "{", "}", "}", "{",
        "{", "}", "}", "{", "{", "{", "}", "}", "}"
    };
    QCOMPARE(t.cachedTexts, expectedTokenTexts);
}

void TokenizerTests::headerCommentsMasking()
{
    //          1;
    //          // 2
    //          3 // 4
    //          5 /* 6
    //          7
    //          8*/ 9
    HeaderTokenizer t;
    t.tokenizeAndCache(
                "#1\n"
                "// #2\n"
                "//\n"
                "#3 // #4\n"
                "#5 /* #6\n"
                "#7\n"
                "#8 */ #9");
    const QStringList expectedTokenTexts {
        "#1", "// #2", "//", "#3", "// #4", "#5", "/* #6\n#7\n#8 */", "#9"
    };
    const QVector<int> expectedTokenTypes {
        5, 1, 1, 5, 1, 5, 0, 5
    };
    QCOMPARE(t.cachedTexts, expectedTokenTexts);
    QCOMPARE(t.cachedTypes, expectedTokenTypes);
}

TokenizerTests::HeaderTokenizer::HeaderTokenizer() :
    tokenizer(Tokenizer::headerTokenizer())
{
}

TokenizerTests::HeaderTokenizer::~HeaderTokenizer()
{
}

void TokenizerTests::HeaderTokenizer::tokenizeAndCache(const PlainFileData &data)
{
    cachedTokens = tokenizer.tokenize(data);
    cachedTypes.clear();
    cachedTexts.clear();
    for (const Token &token : cachedTokens){
        cachedTypes << token.type;
        cachedTexts << token.text;
    }
}
