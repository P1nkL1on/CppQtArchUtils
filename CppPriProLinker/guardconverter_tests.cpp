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
    QCOMPARE(GuardcCnverter::convert("HighCamelCase"), QString("HIGH_CAMEL_CASE"));
    QCOMPARE(GuardcCnverter::convert("low"), QString("LOW"));
    QCOMPARE(GuardcCnverter::convert("Editor5"), QString("EDITOR_5"));
    QCOMPARE(GuardcCnverter::convert("File10"), QString("FILE_10"));
    QCOMPARE(GuardcCnverter::convert("10File10"), QString("10_FILE_10"));
    QCOMPARE(GuardcCnverter::convert("High123High"), QString("HIGH_123_HIGH"));
    QCOMPARE(GuardcCnverter::convert("High123low"), QString("HIGH_123_LOW"));
    QCOMPARE(GuardcCnverter::convert("onestringname"), QString("ONESTRINGNAME"));
    QCOMPARE(GuardcCnverter::convert("CAPS"), QString("CAPS"));
    QCOMPARE(GuardcCnverter::convert("Project2App3"), QString("PROJECT_2_APP_3"));
    QCOMPARE(GuardcCnverter::convert("Project2App3"), QString("PROJECT_2_APP_3"));
    QCOMPARE(GuardcCnverter::convert("GuardConverter_Tests"), QString("GUARD_CONVERTER_TESTS"));
}

void GuardConverterTests::fileExtConvertions()
{

}