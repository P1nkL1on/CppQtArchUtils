#include "file_scanner.h"

FileScanner::FileScanner()
{

}

File *FileScanner::parseFile(
        const QString &filePath,
        const FileFactory *reader,
        QString &err)
{
    const QString ext = QFileInfo(filePath).suffix();
    return reader->read(filePath, err);
}
