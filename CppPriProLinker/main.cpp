#include "form.h"
#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Form w;
//    w.show();

//    return a.exec();
//}


#include "templateloader_tests.h"

#include <QTest>

int main(int argc, char *argv[])
{
    TemplateLoaderTests tests;
    QTest::qExec(&tests, argc, argv);
}
