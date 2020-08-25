#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "file_data.h"
#include "file_link.h"

class FileParser
{
public:
    static bool readFileData(
            const QString &filePath,
            FileData &data,
            QString &errMessage);

    static void parseHeader(
            const FileData &data,
            QVector<FileLink> &links,
            QStringList &classes,
            QString &guard);

    static void parsePri(
            const FileData &data,
            QVector<FileLink> &links);
};

#endif // FILEPARSER_H
