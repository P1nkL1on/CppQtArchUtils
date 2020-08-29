#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "file_factory.h"
#include "thread_worker.h"

class FileScanner
{
public:
    FileScanner();
    ~FileScanner();
    ThreadWorker *createParserWorker(
            const QString &dir,
            const QStringList &fileTypes
            = {"*.cpp", "*.h", "*.pri", "*.pro"});
    static File *parseFile(
            const QString &filePath,
            const FileFactory *reader,
            QString &err);
};

#endif // FILESCANNER_H
