#include "template_decoder_tests.h"

#include "template_decoder.h"

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

void TemplateDecoderTests::listTest()
{
    FileData data {
        "start",
        "$REPEAT",
        ".",
        "$LIST",
        "..",
        "$END",
        "end"
    };
    const QHash<QString, QStringList> varToListHash {
        {"$LIST", {"value1", "value2"}},
    };
    TemplateDecoder::replaceLists(data, varToListHash);
    const FileData expectedData {
        "start",
        ".",
        "value1",
        "..",
        ".",
        "value2",
        "..",
        "end"
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::listReversedTest()
{
    FileData data {
        "$REPEAT_BACKWARD",
        "/Folder/$LIST.png",
        "$END"
    };
    const QHash<QString, QStringList> varToListHash {
        {"$LIST", {"flower", "house", "cat"}}
    };
    TemplateDecoder::replaceLists(data, varToListHash);
    const FileData expectedData {
        "/Folder/cat.png",
        "/Folder/house.png",
        "/Folder/flower.png"
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::multiListTest()
{
    FileData data {
        "$REPEAT",
        "($ALIST + $BLIST),",
        "$END"
    };
    const QHash<QString, QStringList> varToListHash {
        {"$ALIST", {"1", "2", "3"}},
        {"$BLIST", {"4", "5", "6"}},
    };
    TemplateDecoder::replaceLists(data, varToListHash);
    const FileData expectedData {
        "(1 + 4),",
        "(2 + 5),",
        "(3 + 6),"
    };
    QCOMPARE(data, expectedData);
}

void TemplateDecoderTests::severtalListTest()
{
    FileData data {
        "$REPEAT",
        "1. $LIST",
        "$END",
        "$REPEAT",
        "2. $BIGLIST",
        "$END",
        "$REPEAT",
        "3. $LIST",
        "$END",
    };
    const QHash<QString, QStringList> varToListHash {
        {"$LIST", {"a", "b"}},
        {"$BIGLIST", {"c", "d", "e", "f"}},
    };
    TemplateDecoder::replaceLists(data, varToListHash);
    const FileData expectedData {
        "1. a",
        "1. b",
        "2. c",
        "2. d",
        "2. e",
        "2. f",
        "3. a",
        "3. b"
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

void TemplateDecoderTests::fullDecodeTest()
{
    const FileData data {
        "$INC",
        "$START",
        "$CYCLE_START",
        "$OP",
        "$CYCLE_END",
        "$END"
    };
    const QHash<QString, QString> varToStringHash {
        {"$LETTER", "a"},
        {"$INC", "#include <QtDebug>"},
        {"$END", "// end"}
    };
    const QHash<QString, QStringList> varToListHash {
        {"$ITERATOR", {"i", "j", "k"}},
    };
    const QHash<QString, QStringList> varToTemplateHash {
        {"$START", {"$REPEAT_BACKWARD", "auto $ITERATOR = 0;", "$END"}},
        {"$OP", {"qDebug() << '$LETTER';"}},
        {"$CYCLE_START", {"$REPEAT", "for ($ITERATOR = -1; $ITERATOR < 10; ++$ITERATOR) {", "$END"}},
        {"$CYCLE_END", {"$REPEAT_BACKWARD", "} // for $ITERATOR", "$END"}}
    };
    const FileData actualData = TemplateDecoder::decode(data, varToTemplateHash, varToListHash, varToStringHash);
    const FileData expectedData {
        "#include <QtDebug>",
        "auto k = 0;",
        "auto j = 0;",
        "auto i = 0;",
        "for (i = -1; i < 10; ++i) {",
        "for (j = -1; j < 10; ++j) {",
        "for (k = -1; k < 10; ++k) {",
        "qDebug() << 'a';",
        "} // for k",
        "} // for j",
        "} // for i",
        "// end"
    };
    QCOMPARE(actualData, expectedData);
}
