#ifndef FILE_LINK_H
#define FILE_LINK_H

#include <QString>

class File;

struct RefFile
{
    RefFile() = default;
    RefFile(const int pos,
            const QString &text,
            bool isSystem = false);
    bool operator==(
            const RefFile &other) const;
    operator QString() const;
    int pos = -1;
    QString text;
    bool isSystem = false;
};

#endif // FILE_LINK_H
