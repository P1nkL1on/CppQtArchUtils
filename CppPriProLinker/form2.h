#ifndef FORM2_H
#define FORM2_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QListWidget>
#include "file_scanner.h"
#include "textbrowser_links.h"

class Token;
class Tokenizer;

class Form2 : public QMainWindow
{
    Q_OBJECT

public:
    Form2(FileScanner *scanner,
          const QStringList &nameFilters,
          const QStringList &nameIgnore,
          QWidget *parent = nullptr);
    ~Form2();

    void addLinkerLookUpFolder(const QString &dir);
    void run(const QStringList &filePathes, const QString &lookUpFolder);
protected:
    void onRefClicked(const RefFile &refFile);
    void onListItemSelectionChanged();
    QStringList m_lookUpFolder;
    QStringList m_parsedFiles;
    QString m_currentFilePath;

    FileScanner *m_scanner = nullptr;

    QListWidget *m_filesList = nullptr;
    QPlainTextEdit *m_refText = nullptr;
    TextBrowserLinks *m_resultText2 = nullptr;
};

#endif // FORM2_H
