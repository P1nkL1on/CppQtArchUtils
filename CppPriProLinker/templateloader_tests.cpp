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
    QCOMPARE(data, QStringList());
    QCOMPARE(errMsg, QString());
}

void TemplateLoaderTests::readMultiLine()
{

}

void TemplateLoaderTests::readOneLine()
{

}

void TemplateLoaderTests::readUnexist()
{

}
