#ifndef FORM2_H
#define FORM2_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QListWidget>
#include "file_scanner.h"

class Token;
class Tokenizer;

class Form2 : public QMainWindow
{
    Q_OBJECT

public:
    Form2(FileScanner *scanner, Linker *linker,
          const QStringList &nameFilters,
          const QStringList &nameIgnore,
          QWidget *parent = nullptr);
    ~Form2();

    void addLinkerLookUpFolder(const QString &dir);
    void run(const QString &dir);
protected:
    void onLinkHighlighted(const QUrl &url);
    void onLinkClicked(const QUrl &url);
    void onListItemSelectionChanged();
    QStringList m_nameFilters;
    QStringList m_nameIgnore;
    QStringList m_parsedFiles;

    FileScanner *m_scanner = nullptr;
    Linker *m_linker = nullptr;

    QListWidget *m_filesList = nullptr;
    QPlainTextEdit *m_fileText = nullptr;
    QTextBrowser *m_resultText = nullptr;
    QPlainTextEdit *m_tokenText = nullptr;
};

#endif // FORM2_H
