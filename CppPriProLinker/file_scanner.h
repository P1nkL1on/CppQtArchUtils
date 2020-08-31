#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "file_factory.h"
#include "thread_worker.h"

class FileScanner
{
public:
    FileScanner();
    ~FileScanner();
    void parseDir(const QString &dir,
            const QStringList &fileWildCards,
            QWidget *dialogParent = nullptr);
protected:
    QHash<QString, FileFactory *> m_extToFileFactoryHash;
};

#endif // FILESCANNER_H
