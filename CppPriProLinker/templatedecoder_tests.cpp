#include "templatedecoder_tests.h"

#include "templatedecoder.h"

#include <QtTest>

TemplateDecoderTests::TemplateDecoderTests(QObject *parent) :
    QObject(parent)
{
}

TemplateDecoderTests::~TemplateDecoderTests()
{
}

void TemplateDecoderTests::varTest()
{
    FileData data {
        "sep sent /w $VAR_NAME",
        "$VAR_NAME",
        "__$VAR_NAME",
        "$$$VAR_NAME$$$",
        "two in one $VAR_NAME and $VAR_NAME",
        ""
    };
    const QHash<QString, QString> varToStringHash {{"$VAR_NAME", "name"}};
    TemplateDecoder::replaceVars(data, varToStringHash);
    const FileData expectedData {
        "sep sent /w name",
        "name",
        "__name",
        "$$name$$$",
        "two in one name and name",
        ""
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::multiVarTest()
{
    FileData data {
        "$A, $B, $C, $D",
        "$E, $F"
    };
    const QHash<QString, QString> varToStringHash {
        {"$A", "a"},
        {"$C", "c"},
        {"$D", "d"},
        {"$E", "e"},
        {"$F", "f"}
    };
    TemplateDecoder::replaceVars(data, varToStringHash);
    const FileData expectedData {
        "a, $B, c, d",
        "e, f"
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::templateTest()
{
    FileData data {
        "1",
        "$2",
        "3"
    };
    const QHash<QString, QStringList> varToTemplateHash {
        {"$2", {"4", "5", ""}}
    };
    TemplateDecoder::replaceTemplate(data, varToTemplateHash);
    const FileData expectedData {
        "1",
        "4",
        "5",
        "",
        "3"
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::emptyTemplateTest()
{
    FileData data {
        "1",
        "$2",
        "3"
    };
    const QHash<QString, QStringList> varToTemplateHash {{"$2", QStringList()}};
    TemplateDecoder::replaceTemplate(data, varToTemplateHash);
    const FileData expectedData {
        "1",
        "3"
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::multiTemplateTest()
{
    FileData data {
        "$REPLACE",
        "..",
        "$REPLACE",
        ""
    };
    const QHash<QString, QStringList> varToTemplateHash {
        {"$REPLACE", {"line1", "line2"}}
    };
    TemplateDecoder::replaceTemplate(data, varToTemplateHash);
    const FileData expectedData {
        "line1",
        "line2",
        "..",
        "line1",
        "line2",
        ""
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::severalTemplateTest()
{
    FileData data {
        "$REPLACE_1",
        "..",
        "$REPLACE_2",
        ""
    };
    const QHash<QString, QStringList> varToTemplateHash {
        {"$REPLACE_1", {"line1", "line2"}},
        {"$REPLACE_2", {"line3", "line4"}}
    };
    TemplateDecoder::replaceTemplate(data, varToTemplateHash);
    const FileData expectedData {
        "line1",
        "line2",
        "..",
        "line3",
        "line4",
        ""
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::templateToVarTest()
{
    FileData data {
        "$NAME",
        "{",
        "$OP",
        "}"
    };
    const QHash<QString, QString> varToStringHash {
        {"$NAME", "test_name"},
    };
    const QHash<QString, QStringList> varToTemplateHash {
        {"$OP", {"$NAME = ok;", "return $NAME + $NAME;"}},
    };
    TemplateDecoder::replaceTemplate(data, varToTemplateHash);
    TemplateDecoder::replaceVars(data, varToStringHash);
    const FileData expectedData {
        "test_name",
        "{",
        "test_name = ok;",
        "return test_name + test_name;",
        "}"
    };
    QCOMPARE(data, expectedData);
}
