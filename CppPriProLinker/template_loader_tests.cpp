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
    FileData data;
    QString errMsg;
    QVERIFY(TokenParser::readFileData(":/empty.txt", data, errMsg));
    QCOMPARE(errMsg, QString());
    QCOMPARE(data, QStringList());
}

void TemplateLoaderTests::readMultiLine()
{
    FileData data;
    QString errMsg;
    QVERIFY(TokenParser::readFileData(":/multiline.txt", data, errMsg));
    QCOMPARE(errMsg, QString());
    const QStringList expectedData = {
        "usuall line",
        "\ttabbed line",
        "",
        "^ empty line",
        "\t\t\t\t\t",
        "^ tabbled empty line",
        "\tv also empty"
    };
    QCOMPARE(data, expectedData);
}

void TemplateLoaderTests::readOneLine()
{
    FileData data;
    QString errMsg;
    QVERIFY(TokenParser::readFileData(":/oneline.txt", data, errMsg));
    QCOMPARE(errMsg, QString());
    QCOMPARE(data, QStringList{"\tany one line\t  \t"});
}

void TemplateLoaderTests::readUnexist()
{
    FileData data;
    QString errMsg;
    QVERIFY(not TokenParser::readFileData(":/none.txt", data, errMsg));
    QVERIFY(not errMsg.isEmpty());
}
