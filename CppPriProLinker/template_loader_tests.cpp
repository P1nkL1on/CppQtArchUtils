#include "template_loader_tests.h"

#include "template_loader.h"

#include <QtTest>

TemplateLoaderTests::TemplateLoaderTests(QObject *parent) :
    QObject(parent)
{
}

TemplateLoaderTests::~TemplateLoaderTests()
{
}

void TemplateLoaderTests::readEmptyFile()
{
    PlainFileData data;
    QString errMsg;
    QVERIFY(TokenParser::readPlainFileData(":/empty.txt", data, errMsg));
    QCOMPARE(errMsg, QString());
    QCOMPARE(data, PlainFileData());
}

void TemplateLoaderTests::readMultiLine()
{
    PlainFileData data;
    QString errMsg;
    QVERIFY(TokenParser::readPlainFileData(":/multiline.txt", data, errMsg));
    QCOMPARE(errMsg, QString());
    const PlainFileData expectedData(
        "usuall line\n"
        "\ttabbed line\n"
        "\n"
        "^ empty line\n"
        "\t\t\t\t\t\n"
        "^ tabbled empty line\n"
        "\tv also empty"
    );
    QCOMPARE(data, expectedData);
}

void TemplateLoaderTests::readOneLine()
{
    PlainFileData data;
    QString errMsg;
    QVERIFY(TokenParser::readPlainFileData(":/oneline.txt", data, errMsg));
    QCOMPARE(errMsg, QString());
    QCOMPARE(data, PlainFileData("\tany one line\t  \t"));
}

void TemplateLoaderTests::readUnexist()
{
    PlainFileData data;
    QString errMsg;
    QVERIFY(not TokenParser::readPlainFileData(":/none.txt", data, errMsg));
    QVERIFY(not errMsg.isEmpty());
}
