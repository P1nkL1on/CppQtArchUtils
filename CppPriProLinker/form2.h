#ifndef FORM2_H
#define FORM2_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QListWidget>

class Token;
class Tokenizer;

class Form2 : public QMainWindow
{
    Q_OBJECT

public:
    Form2(QWidget *parent = nullptr);
    ~Form2();
    static QVector<Token> tokenize(const QString &filePath, const Tokenizer &tokenizer, int &loadingTime, int &tokenizingTime);
protected:
    QListWidget *m_filesList = nullptr;
    QPlainTextEdit *m_fileText = nullptr;
    QPlainTextEdit *m_resultText = nullptr;
    QPlainTextEdit *m_tokenText = nullptr;
};

#endif // FORM2_H
