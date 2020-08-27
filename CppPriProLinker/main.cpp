#include "form2.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form2 w;
    w.showMaximized();

    return a.exec();
}


//#include "template_loader_tests.h"
//#include "template_decoder_tests.h"
//#include "guard_converter_tests.h"
//#include "tokenizer_tests.h"

//#include <QTest>

//int main(int argc, char *argv[])
//{
//    TemplateLoaderTests testsLoader;
//    QTest::qExec(&testsLoader, argc, argv);

//    TemplateDecoderTests testsDecoder;
//    QTest::qExec(&testsDecoder, argc, argv);

//    GuardConverterTests testsGuards;
//    QTest::qExec(&testsGuards, argc, argv);

//    TokenizerTests testsTokenizer;
//    QTest::qExec(&testsTokenizer, argc, argv);
//}

