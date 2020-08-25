#include "form.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form w;
    w.show();

    return a.exec();
}


//#include "templateloader_tests.h"
//#include "templatedecoder_tests.h"
//#include "guardconverter_tests.h"

//#include <QTest>

//int main(int argc, char *argv[])
//{
//    TemplateLoaderTests testsLoader;
//    QTest::qExec(&testsLoader, argc, argv);

//    TemplateDecoderTests testsDecoder;
//    QTest::qExec(&testsDecoder, argc, argv);

//    GuardConverterTests testsGuards;
//    QTest::qExec(&testsGuards, argc, argv);
//}

