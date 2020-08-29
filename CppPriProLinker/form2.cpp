#include "form2.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QDebug>

#include "file_cpp_factory.h"
#include "file_pro_factory.h"
#include "dialog_progress.h"

Form2::Form2(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    centralWidget->setLayout(layout);
    layout->addWidget(m_filesList = new QListWidget, 1);
    m_filesList->setFocusPolicy(Qt::NoFocus);

//    layout->addWidget(m_fileText = new QPlainTextEdit, 1);
//    m_fileText->setReadOnly(true);

    layout->addWidget(m_resultText = new QPlainTextEdit, 1);
    m_resultText->setReadOnly(true);

//    layout->addWidget(m_tokenText = new QPlainTextEdit, 1);
//    m_tokenText->setReadOnly(true);
    setCentralWidget(centralWidget);


    auto pris = new FileProFactory;
    auto cpps = new FileCppFactory;
    m_extToFileFactoryHash = {
        {"h", cpps},
        {"cpp", cpps},
        {"pri", pris},
        {"pro", pris}
    };
}

Form2::~Form2()
{
}

void Form2::startDeprecated()
{
//    Tokenizer tokenizer = Tokenizer::proTokenizer();
    QDirIterator dirIterator(
                "/home/alex/R3DS/Applications",
//                "/home/alex/jff/",
                QStringList{"*.pri", "*.pro"},
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
//        m_fileText->clear();
        m_resultText->clear();


//        PlainFileData text;
//        QString err;
//        if (not TokenParser::readPlainFileData(filePath, text, err)){
//            m_resultText->setPlainText(QString("Loading error: %1").arg(err));
//            return;
//        }
//        const QVector<Token> tokens = tokenizer.tokenize(text);

//        QString guard;
//        QVector<RefFile> includes;
//        QVector<RefClass> classes;
//        TokenParser::parseCpp(tokens, includes, classes, guard);

//        QString includesStr;
//        for (const RefFile &i: includes)
//            includesStr += "    " + i + "\n";
//        QString classesStr;
//        for (const RefClass &i: classes)
//            classesStr += "    " + i + "\n";
//        m_resultText->setPlainText(QString("Guard: %1\n\nLinks:\n%2\nClasses:\n%3")
//                                   .arg(guard).arg(includesStr).arg(classesStr));

//        QVector<RefFile> includes;
//        QVector<ProConfig> configs;
//        TokenParser::parsePro(tokens, configs, includes);
//        QString includesStr;
//        for (const RefFile &i: includes)
//            includesStr += "    " + i + "\n";
//        m_resultText->setPlainText(QString("Links:\n%1").arg(includesStr));

//        m_fileText->setPlainText(text);

//        QString tokensStr;
//        for (const Token &token : tokens)
//            tokensStr += QString("%1:  \t%2\n")
//                    .arg(int(token.type))
//                    .arg(token.text);

//        m_tokenText->setPlainText(tokensStr);
    });
}

File * Form2::read(
        const QString &filePath,
        QString &err,
        int &readingTime,
        int &tokenizeTime,
        int &parsingTime) const
{
}

void Form2::run(const QString &dir)
{
    Dialog *d = new Dialog(this);
    d->setOkCancel();
    qDebug() << d->exec();
    return;

    QStringList extMasks;
    for (const QString &s : m_extToFileFactoryHash.keys())
        extMasks << "*." + s;
    QDirIterator dirIterator(
                dir,
                extMasks,
                QDir::Files,
                QDirIterator::Subdirectories);
    while (dirIterator.hasNext()){
        const QString path = dirIterator.next();
        QString err;
        int a, b, c;
        File *file = read(path, err, a, b, c);
        m_absPathToFileHash.insert(path, file);
        if (file == nullptr){
            m_absPathToFilePreview.insert(path, err);
            continue;
        }
        QString preview;
        for (const RefFile &ref : file->refs.keys()){
            File *f = file->refs.value(ref);
            preview += QString("%1 -> %2\n")
                    .arg(ref).arg(f ? "ok" : "?");
        }
        m_absPathToFilePreview.insert(path, preview);
    }
    m_filesList->addItems(m_absPathToFilePreview.keys());
    connect(m_filesList, &QListWidget::itemPressed, this, [=](QListWidgetItem *item){
        m_resultText->setPlainText(m_absPathToFilePreview.value(item->text()));
    });
}

QVector<Token> Form2::tokenize(
        const QString &filePath,
        const Tokenizer &tokenizer,
        int &loadingTime,
        int &tokenizingTime)
{
//    QElapsedTimer timer;
//    timer.start();
//    PlainFileData text;
//    QString err;
//    const bool isOk = TokenParser::readPlainFileData(filePath, text, err);
//    loadingTime = timer.elapsed();
//    Q_ASSERT(isOk);

//    timer.restart();
//    const QVector<Token> tokens = tokenizer.tokenize(text);
//    tokenizingTime = timer.elapsed();
//    return tokens;
}
