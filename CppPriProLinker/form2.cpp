#include "form2.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QDebug>
#include "token_parser.h"
#include "tokenizer.h"

Form2::Form2(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    centralWidget->setLayout(layout);
    layout->addWidget(m_filesList = new QListWidget, 1);
    m_filesList->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(m_fileText = new QPlainTextEdit, 1);
    m_fileText->setReadOnly(true);

    layout->addWidget(m_resultText = new QPlainTextEdit, 1);
    m_resultText->setReadOnly(true);

    layout->addWidget(m_tokenText = new QPlainTextEdit, 1);
    m_tokenText->setReadOnly(true);

    Tokenizer tokenizer = Tokenizer::headerTokenizer();
    QDirIterator dirIterator(
                "",
                QStringList{"*.h", "*.cpp"},
                QDir::Files,
                QDirIterator::Subdirectories);

//    int totalLoadingTime = 0;
//    int totalTokenizingTime = 0;
//    while (dirIterator.hasNext()){
//        const auto path = dirIterator.next();
//        int loadingTime, tokenizingTime;
//        tokenize(path, tokenizer, loadingTime, tokenizingTime);

//        totalLoadingTime += loadingTime;
//        totalTokenizingTime += tokenizingTime;
//        qDebug() << dirIterator.fileInfo().fileName() << "Loading:" << loadingTime << "Tokenizing:" << tokenizingTime;
//    }
//    qDebug() << "Total loading:" << totalLoadingTime << "Total tokenizing:" << totalTokenizingTime;
//    return;

    QStringList filePathes;
    while (dirIterator.hasNext())
        filePathes << dirIterator.next();

    m_filesList->addItems(filePathes);
    connect(m_filesList, &QListWidget::itemPressed, this, [=](QListWidgetItem *item){
        m_fileText->clear();
        m_resultText->clear();
        const QString filePath = item->text();
        PlainFileData text;
        QString err;
        if (not TokenParser::readPlainFileData(filePath, text, err)){
            m_resultText->setPlainText(QString("Loading error: %1").arg(err));
            return;
        }
        const QVector<Token> tokens = tokenizer.tokenize(text);

        QString guard;
        QVector<RefFile> includes;
        QVector<RefClass> classes;
        TokenParser::parseCpp(tokens, includes, classes, guard);

        QString includesStr;
        for (const RefFile &i: includes)
            includesStr += "    " + i + "\n";
        QString classesStr;
        for (const RefClass &i: classes)
            classesStr += "    " + i + "\n";
        m_resultText->setPlainText(QString("Guard: %1\n\nLinks:\n%2\nClasses:\n%3")
                                   .arg(guard).arg(includesStr).arg(classesStr));

        m_fileText->setPlainText(text);

        QString tokensStr;
        for (const Token &token : tokens)
            tokensStr += QString("%1:  \t%2\n")
                    .arg(int(token.type))
                    .arg(token.text);

        m_tokenText->setPlainText(tokensStr);
    });

    setCentralWidget(centralWidget);
}

Form2::~Form2()
{
}

QVector<Token> Form2::tokenize(const QString &filePath, const Tokenizer &tokenizer, int &loadingTime, int &tokenizingTime)
{
    QElapsedTimer timer;
    timer.start();
    PlainFileData text;
    QString err;
    const bool isOk = TokenParser::readPlainFileData(filePath, text, err);
    loadingTime = timer.elapsed();
    Q_ASSERT(isOk);

    timer.restart();
    const QVector<Token> tokens = tokenizer.tokenize(text);
    tokenizingTime = timer.elapsed();
    return tokens;
}
