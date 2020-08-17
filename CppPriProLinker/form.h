#ifndef FORM_H
#define FORM_H

#include <QMainWindow>

class Form : public QMainWindow
{
    Q_OBJECT

public:
    Form(QWidget *parent = 0);
    ~Form();
};

#endif // FORM_H
