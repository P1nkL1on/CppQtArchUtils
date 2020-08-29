#ifndef FORM2_H
#define FORM2_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QListWidget>
#include "file_scanner.h"

class Token;
class Tokenizer;

class Form2 : public QMainWindow
{
    Q_OBJECT

public:
    Form2(QWidget *parent = nullptr);
    ~Form2();
    void run(
            const QString &dir,
            const QStringList &fileWildCards);
//    File *read(
//            const QString &filePath,
//            QString &err,
//            int &readingTime,
//            int &tokenizeTime,
//            int &parsingTime) const;
//    void run(const QString &dir);
protected:
//    QHash<QString, FileFactory *> m_extToFileFactoryHash;
//    QHash<QString, File *> m_absPathToFileHash;
//    QMap<QString, QString> m_absPathToFilePreview;

    QListWidget *m_filesList = nullptr;
    QPlainTextEdit *m_fileText = nullptr;
    QPlainTextEdit *m_resultText = nullptr;
    QPlainTextEdit *m_tokenText = nullptr;
private:
//    void startDeprecated();
//    static QVector<Token> tokenize(
//            const QString &filePath,
//            const Tokenizer &tokenizer,
//            int &loadingTime,
//            int &tokenizingTime);
};

#endif // FORM2_H
