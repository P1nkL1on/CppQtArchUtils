#ifndef FILEIO_H
#define FILEIO_H

#include "file_data.h"

class FileIO
{
public:
    // move to filereader utils
    static bool readPlainFileData(
            const QString &filePath,
            PlainFileData &data,
            QString &errMessage);
};

#endif // FILEIO_H
