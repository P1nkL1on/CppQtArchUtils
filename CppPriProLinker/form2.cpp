#include "form2.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QDebug>
#include <QApplication>
#include "token_parser.h"

Form2::Form2(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    centralWidget->setLayout(layout);
    layout->addWidget(m_filesList = new QListWidget, 1);
    m_filesList->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(m_resultText = new QTextBrowser, 1);
    m_resultText->setReadOnly(true);

    setCentralWidget(centralWidget);
}

Form2::~Form2()
{
}

void Form2::run(
        const QString &dir,
        const QString &lookUpDir,
        const QStringList &fileWildCards,
        const QStringList &ignorePathes)
{
    FileScanner *scanner = new FileScanner(fileWildCards, ignorePathes);
    const QStringList allPathes = scanner->scanDir(lookUpDir);
    scanner->addScannedFilePathes(allPathes);

    const QStringList wantPathes = scanner->scanDir(dir);



    QStringList toParse = {dir};
    scanner->cacheFiles(toParse);
    scanner->link(toParse);

//    while (1){
//        const int cached = scanner.cacheFiles(toParse);
//        if (not cached)
//            break;
//        toParse.clear();
//        scanner.link(toParse);
//        QApplication::processEvents();
//    }

    QStringList *cachedPathes = new QStringList;
    for (const QString &path : scanner->scannedFilePathes())
        if (scanner->isCached(path))
            cachedPathes->append(path);

    m_filesList->addItems(*cachedPathes);
    connect(m_filesList, &QListWidget::itemSelectionChanged, this, [=]{
        if (m_filesList->currentRow() < 0){
            m_resultText->clear();
            return;
        }
        const QString filePath = m_filesList->currentItem()->text();
        const auto hash = scanner->preLinkHash(filePath);

        QString s;
        QString err;
        TokenParser::readPlainFileData(filePath, s, err);

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
            format.setFont(QFont("Courier New", 20));
            m_resultText->setCurrentCharFormat(format);
            cursor.insertText(text, format);
            prev = pos;
        }
    });

    connect(m_resultText, QOverload<const QString &>::of(&QTextBrowser::highlighted), this, [&](const QString &r){
        qDebug() << r;
    });
    connect(m_resultText, &QTextBrowser::anchorClicked, this, [=](const QUrl &r){
        const QStringList alreadyCached = *cachedPathes;
        const QString url = r.url();
        int index = alreadyCached.indexOf(url);
        if (index < 0){
            QStringList toParse = {url};
            scanner->cacheFiles(toParse);
            scanner->link(toParse);
            QStringList *cachedPathes = new QStringList;
            for (const QString &path : scanner->scannedFilePathes())
                if (scanner->isCached(path))
                    cachedPathes->append(path);
            m_filesList->clear();
            m_filesList->addItems(*cachedPathes);
            index = cachedPathes->indexOf(url);
        }
        m_filesList->setCurrentRow(index);
    });
    m_resultText->setOpenLinks(false);
    m_resultText->setOpenExternalLinks(false);

}
