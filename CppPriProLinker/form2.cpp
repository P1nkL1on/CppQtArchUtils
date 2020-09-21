#include "form2.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QDebug>
#include <QApplication>
#include "file_io.h"

Form2::Form2(
        FileScanner *scanner,
        Linker *linker,
        const QStringList &nameFilters,
        const QStringList &nameIgnore,
        QWidget *parent) :
    QMainWindow(parent),
    m_nameFilters(nameFilters),
    m_nameIgnore(nameIgnore),
    m_scanner(scanner),
    m_linker(linker)
{
    // layout
    QWidget *centralWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    centralWidget->setLayout(layout);
    layout->addWidget(m_filesList = new QListWidget, 1);
    connect(m_filesList, &QListWidget::itemSelectionChanged, this, &Form2::onListItemSelectionChanged);

    m_filesList->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *right = new QVBoxLayout;

    m_resultText2 = new TextBrowserLinks;
    // connect(m_resultText2, &TextBrowserLinks::refHighLighted, this, &Form2::onRefHighLighted);
    connect(m_resultText2, &TextBrowserLinks::refClicked, this, &Form2::onRefClicked);
    right->addWidget(m_resultText2, 1);

    m_refText = new QPlainTextEdit;
    m_refText->setReadOnly(true);
    m_refText->setFocusPolicy(Qt::NoFocus);
    right->addWidget(m_refText);

    layout->addLayout(right, 1);

    setCentralWidget(centralWidget);
}

Form2::~Form2()
{
}

void Form2::addLinkerLookUpFolder(const QString &dir)
{
    QFileInfo info(dir);
    Q_ASSERT(info.isDir());

    const QStringList pathes = m_scanner->filePathesInDir(
                dir, m_nameFilters, m_nameIgnore);
    m_linker->addFilePathes(pathes);
}

void Form2::run(const QStringList &filePathes)
{
    m_scanner->parseFiles(filePathes, [=]{
        QStringList yetToParse;
        m_scanner->tryLinkRefs(yetToParse);

        // stop when 0 new parsed at least once
//        const auto newPathes = m_scanner->parsedFilePathes();
//        if (newPathes.size() == m_parsedFiles.size())
//            return;
//        m_parsedFiles = newPathes;

        m_parsedFiles = m_scanner->parsedFilePathes();
        m_filesList->clear();
        m_filesList->addItems(m_parsedFiles);

//        if (not yetToParse.isEmpty())
//            run(yetToParse);
    }, FileScanner::InBackground);
}

void Form2::onRefClicked(const RefFile &refFile)
{
    if (m_currentFilePath.isEmpty())
        return;
    const QHash<RefFile, QString> hash = m_scanner->parsedRefHash(m_currentFilePath);
    const QString text = hash.value(refFile);
    if (text.isEmpty())
        return;
    int index = m_parsedFiles.indexOf(text);
    if (index >= 0){
        m_filesList->setCurrentRow(index);
        return;
    }
    run({text});
}

void Form2::onListItemSelectionChanged()
{
    if (m_filesList->currentRow() < 0){
        m_currentFilePath.clear();
        m_resultText2->clear();
        return;
    }
    const QString filePath = m_filesList->currentItem()->text();
    const QHash<RefFile, QString> hash = m_scanner->parsedRefHash(filePath);

    QString s;
    QString err;
    FileIO::readPlainFileData(filePath, s, err);

    m_resultText2->setRefs(hash.keys());
    m_resultText2->setText(s);
    m_currentFilePath = filePath;

    QString refsProblems;
    for (const RefFile &ref : hash.keys()){
        QString s;
        if (ref.isSystem)
            s = ref.text + " is SYSTEM!";
        else if (ref.text.isEmpty())
            s = ref.text + " NOT LINKED!";
        else
            continue;
        refsProblems += s + "\n";
    }
    m_refText->setPlainText(refsProblems);
}
