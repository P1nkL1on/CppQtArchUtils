#include "form2.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QDebug>
#include <QApplication>

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

void Form2::run(const QString &dir, const QString &lookUpDir, const QStringList &fileWildCards)
{
    FileScanner scanner(fileWildCards);
    const QStringList allPathes = scanner.scanDir(lookUpDir);
    scanner.addScannedFilePathes(allPathes);

    const QStringList wantPathes = scanner.scanDir(dir);
    QStringList toParse = wantPathes;

    while (1){
        const int cached = scanner.cacheFiles(toParse);
        if (not cached)
            break;
        toParse.clear();
        scanner.link(toParse);
        QApplication::processEvents();
    }

    QStringList cachedPathes;
    for (const QString &path : scanner.scannedFilePathes())
        if (scanner.isCached(path))
            cachedPathes << path;

    m_filesList->addItems(cachedPathes);
    connect(m_filesList, &QListWidget::itemPressed, this, [=](QListWidgetItem *item){
        const QString filePath = item->text();
        File *f = scanner.cachedFile(filePath);

        QString text = QString("Path: %1\n\nLinks:\n").arg(filePath);
        const auto hash = scanner.preLinkHash(filePath);
        for (const RefFile &ref : hash.keys()){
            const QString path = hash.value(ref);
            const QString str = path.isEmpty() ? "??" : path;
            text += QString("%3\t%1 -> %2\n")
                    .arg(ref.text)
                    .arg(str)
                    .arg(f->m_refToFileHash.value(ref, nullptr) ? "YES" : "");
        }
        m_resultText->setText(text);
    });

}
