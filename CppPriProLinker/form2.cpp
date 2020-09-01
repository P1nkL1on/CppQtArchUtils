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
    layout->addWidget(m_resultText = new QTextBrowser, 1);
    m_resultText->setReadOnly(true);
    m_resultText->setOpenLinks(false);
    m_resultText->setOpenExternalLinks(false);
    connect(m_resultText, QOverload<const QUrl &>::of(&QTextBrowser::highlighted), this, &Form2::onLinkHighlighted);
    connect(m_resultText, &QTextBrowser::anchorClicked, this, &Form2::onLinkClicked);

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

        m_parsedFiles = m_scanner->parsedFilePathes();
        m_filesList->clear();
        m_filesList->addItems(m_parsedFiles);
    }, FileScanner::InBackground);
}

void Form2::onLinkHighlighted(const QUrl &url)
{

}

void Form2::onLinkClicked(const QUrl &url)
{
    const QString text = url.url();
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
        m_resultText->clear();
        return;
    }
    const QString filePath = m_filesList->currentItem()->text();
    const auto hash = m_scanner->parsedRefHash(filePath);

    QString s;
    QString err;
    FileIO::readPlainFileData(filePath, s, err);

    m_resultText->document()->clear();
    QTextCursor cursor(m_resultText->document());
    QMap<int, QString> formats;
    for (const RefFile &ref : hash.keys()){
        QTextCharFormat linkFormat = cursor.charFormat();
        const QString path = hash.value(ref);
        formats.insert(ref.pos, "");
        formats.insert(ref.pos + ref.text.size(), path);
    }
    formats.insert(s.size() - 1, {});
    int prev = 0;
    for (int pos : formats.keys()){
        QTextCharFormat format;
        const QString link = formats.value(pos);
        const QString text = s.mid(prev, pos - prev);
        format.setAnchorHref(link);
        format.setFont(QFont("Courier New", 14));
        m_resultText->setCurrentCharFormat(format);
        cursor.insertText(text, format);
        prev = pos;
    }
}
