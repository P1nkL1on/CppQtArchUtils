#include "templateloader_tests.h"

#include "templateloader.h"

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
    QVERIFY(TemplateLoader::readFileData(":/empty.txt", "\r\n", data, errMsg));
    QCOMPARE(errMsg, QString());
    QCOMPARE(data, QStringList());
}

void TemplateLoaderTests::readMultiLine()
{
    FileData data;
    QString errMsg;
    QVERIFY(TemplateLoader::readFileData(":/multiline.txt", "\r\n", data, errMsg));
    QCOMPARE(errMsg, QString());
    const QStringList expectedData = {
        "usuall line",
        "\ttabbed line",
        "",
        "^ empty line",
        "\t\t\t\t\t",
        "^ tabbled empty line",
        "\tv also empty",
        ""
    };
    QCOMPARE(data, expectedData);
}

void TemplateLoaderTests::readOneLine()
{
    FileData data;
    QString errMsg;
    QVERIFY(TemplateLoader::readFileData(":/oneline.txt", "\r\n", data, errMsg));
    QCOMPARE(errMsg, QString());
    QCOMPARE(data, QStringList{"\tany one line\t  \t"});
}

void TemplateLoaderTests::readUnexist()
{
    FileData data;
    QString errMsg;
    QVERIFY(not TemplateLoader::readFileData(":/none.txt", "\r\n", data, errMsg));
    QVERIFY(not errMsg.isEmpty());
}
