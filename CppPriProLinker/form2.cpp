#include "form2.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include "file_parser.h"
#include "file_tokenizer.h"

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

    QDirIterator dirIterator(
                "D:\\R3DS\\Prohor",
                QStringList{"*.h"},
                QDir::Files,
                QDirIterator::Subdirectories);
    QStringList filePathes;
    while (dirIterator.hasNext())
        filePathes << dirIterator.next();


    const QStringList tokenNames {
        "Comment",
        "Qoute",
        "Char",
        "Directive",
        "Open Curly",
        "Close Curly",
        "Key Word",
        "Name",
        "Number",
        "White Space"
    };

    m_filesList->addItems(filePathes);
    connect(m_filesList, &QListWidget::itemPressed, this, [=](QListWidgetItem *item){
        m_fileText->clear();
        m_resultText->clear();
        const QString filePath = item->text();
        FileData data;
        QString err;
        if (not FileParser::readFileData(filePath, data, err)){
            m_resultText->setPlainText(QString("Loading error: %1").arg(err));
            return;
        }
        QString guard;
        QVector<FileLink> links;
        QStringList classes;
        FileParser::parseHeader(data, links, classes, guard);

        QString text;
        m_fileText->setPlainText(text = data.join("\n"));

        QVector<Token> tokens = FileTokenizer::tokenize(text);
        QString tokensStr;
        for (const Token &token : tokens)
            tokensStr += QString("%1:  %2\n")
                    .arg(tokenNames.at(token.type))
                    .arg(token.text);
        QString linksStr;
        for (const FileLink &link : links)
            linksStr += QString("    %1\n").arg(link.includePath());
        m_resultText->setPlainText(QString("Guard: '%1'\n\nLinks:\n%2\nClasses:\n    %3")
                                   .arg(guard).arg(linksStr).arg(classes.join("\n    ")));
        m_tokenText->setPlainText(tokensStr);
    });

    setCentralWidget(centralWidget);
}

Form2::~Form2()
{
}
