#ifndef FILEDATA_H
#define FILEDATA_H

#include <QStringList>

using FileData = QStringList;

using PlainFileData = QString;

inline PlainFileData toPlainData(const FileData &data){
    return data.join("\n");
}

#endif // FILEDATA_H
