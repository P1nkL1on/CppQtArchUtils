#include "form2.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    auto l = new Linker;
    auto s = new FileScanner(l);
    const QStringList nameFilters{"*.h", "*.cpp", "*.pro", "*.pri"};
    const QStringList nameIgnores{"build-", "moc_", "3rdparty"};
    Form2 w(s, l, nameFilters, nameIgnores);
    w.showMaximized();
    w.addLinkerLookUpFolder("/home/alex/jff");

    const QStringList files = s->filePathesInDir("/home/alex/jff/CppQtArchUtils/CppPriProLinker", nameFilters, nameIgnores);
//    const QStringList files = {"/home/alex/jff/CppQtArchUtils/CppPriProLinker/CppPriProLinker.pro"};
    w.run(files);
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

