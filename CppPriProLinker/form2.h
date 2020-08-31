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
    Form2(const QStringList &fileWildCards,
          const QStringList &ignorePathes,
          QWidget *parent = nullptr);
    ~Form2();

    void addLinkerLookUpFolder(const QString &dir);
    void run(const QString &dir);
//    void run(
//            const QString &dir,
//            const QString &lookUpDir,
//            );
protected:
    void onLinkHighlighted(const QUrl &url);
    void onLinkClicked(const QUrl &url);
    void onListItemSelectionChanged();
    FileScanner *m_scanner = nullptr;
    QStringList m_parsedFiles;

    QListWidget *m_filesList = nullptr;
    QPlainTextEdit *m_fileText = nullptr;
    QTextBrowser *m_resultText = nullptr;
    QPlainTextEdit *m_tokenText = nullptr;
};

#endif // FORM2_H
