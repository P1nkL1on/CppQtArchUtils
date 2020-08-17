#include "form.h"

#include "templateloader.h"

#include <QDebug>

Form::Form(QWidget *parent)
    : QMainWindow(parent)
{
    TemplateLoader t({{"cpp_0", ":/source_0.txt"}}, "\r\n");
    FileData d;
    QString err;
    const bool isOk = t.readTemplate("cpp_0", d, err);

    if (!isOk)
        qDebug() << err;
    else
        qDebug() << d;
}

Form::~Form()
{

}
