#include "file_io.h"

#include <QFile>
#include <QFileInfo>

bool FileIO::readPlainFileData(
        const QString &filePath,
        PlainFileData &data,
        QString &errMessage)
{
    QFileInfo info(filePath);
    if (not info.exists()){
        errMessage = QString("File doen't exist! %1").arg(filePath);
        return false;
    }
    QFile file(filePath);
    if (not file.open(QIODevice::ReadOnly)){
        errMessage = QString("File can't be readen! %1").arg(filePath);
        return false;
    }
    const QByteArray byteData = file.readAll();
    file.close();
    data = QString(byteData);
    errMessage = QString();
    return true;
}
