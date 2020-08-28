#ifndef REFERENCECLASS_H
#define REFERENCECLASS_H

#include <QStringList>

class File;

struct RefClass
{
    RefClass() = default;
    RefClass(const int pos,
             const QString &name,
             const QStringList &namespaces = {},
             bool definition = false);
    bool operator==(
            const RefClass &other) const;
    operator QString() const;
    int pos = -1;
    QString name;
    QStringList namespaces;
    bool definition = false;
};

#endif // REFERENCECLASS_H