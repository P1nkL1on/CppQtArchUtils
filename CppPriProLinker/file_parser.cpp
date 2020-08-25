#include "file_parser.h"

#include <QFile>
#include <QFileInfo>
#include <QVector>

bool FileParser::readFileData(
        const QString &filePath,
        FileData &data,
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
    while (not file.atEnd())
        data << QString(file.readLine()).replace("\r\n", "");
    file.close();
    errMessage = QString();
    return true;
}

void FileParser::parseHeader(
        const FileData &data,
        QString &guard,
        QVector<FileLink> &links,
        QStringList &classes)
{
    for (const QString &line : data){
        const QString lineTrimmed = line.trimmed();
        if (lineTrimmed.startsWith("#include"))
            links << FileLink(lineTrimmed.split(' ', QString::SkipEmptyParts).last());
        if (lineTrimmed.startsWith("#ifndef"))
            guard = lineTrimmed.split(' ', QString::SkipEmptyParts).last();
        if (lineTrimmed.startsWith("class ") or lineTrimmed.contains(" class ")){
            const QStringList tokens = lineTrimmed.split(' ', QString::SkipEmptyParts);
            classes << tokens.at(tokens.indexOf("class") + 1);
        }
    }
}

void FileParser::parsePri(
        const FileData &data,
        QVector<FileLink> &links)
{

}
