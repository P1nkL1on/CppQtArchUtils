#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "file_factory.h"

class FileScanner
{
public:
    FileScanner();
    ~FileScanner();
    void parseFilesInDir(
            const QString &dir,
            const QStringList &fileTypes
            = {"*.cpp", "*.h", "*.pri", "*.pro"});
    static File *parseFile(
            const QString &filePath,
            const FileFactory *reader,
            QString &err,
            int &readingTime,
            int &tokenizeTime,
            int &parsingTime);
};

#endif // FILESCANNER_H
