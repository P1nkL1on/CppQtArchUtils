#ifndef FORM2_H
#define FORM2_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QListWidget>

class Form2 : public QMainWindow
{
    Q_OBJECT

public:
    Form2(QWidget *parent = nullptr);
    ~Form2();
protected:
    QListWidget *m_filesList = nullptr;
    QPlainTextEdit *m_fileText = nullptr;
    QPlainTextEdit *m_resultText = nullptr;
    QPlainTextEdit *m_tokenText = nullptr;
};

#endif // FORM2_H
