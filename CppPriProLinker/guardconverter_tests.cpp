#include "guardconverter_tests.h"

#include "guardconverter.h"

#include <QTest>

GuardConverterTests::GuardConverterTests(QObject *parent) :
    QObject(parent)
{
}

GuardConverterTests::~GuardConverterTests()
{
}

void GuardConverterTests::expectedConvertions()
{
    QCOMPARE(GuardcCnverter::convert("HighCamelCase.h"), QString("HIGH_CAMEL_CASE_H"));
    QCOMPARE(GuardcCnverter::convert("low.pri"), QString("LOW_PRI"));
    QCOMPARE(GuardcCnverter::convert("onestringname.h"), QString("ONESTRINGNAME_H"));
    QCOMPARE(GuardcCnverter::convert("CAPS.h"), QString("CAPS_H"));
    QCOMPARE(GuardcCnverter::convert("Project2App3.h"), QString("PROJECT_2_APP_3_H"));
    QCOMPARE(GuardcCnverter::convert("Project2App3.h"), QString("PROJECT_2_APP_3_H"));
    QCOMPARE(GuardcCnverter::convert("GuardConverter_Tests.h"), QString("GUARD_CONVERTER_TESTS_H"));
}
