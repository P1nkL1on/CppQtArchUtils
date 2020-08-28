#include "ref_class.h"

RefClass::RefClass(
        const int pos,
        const QString &name,
        const QStringList &namespaces) :
    pos(pos),
    name(name),
    namespaces(namespaces)
{
}

bool RefClass::operator==(
        const RefClass &other) const
{
    return pos == other.pos
            and name == other.name
            and namespaces == other.namespaces;
}

RefClass::operator QString() const
{
    QStringList tmp = namespaces;
    tmp << name;
    return QString("(%2) %1")
            .arg(tmp.join("::"))
            .arg(pos);
}
