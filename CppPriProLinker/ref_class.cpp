#include "ref_class.h"

RefClass::RefClass(
        const int pos,
        const QString &name,
        const QStringList &namespaces,
        bool definition) :
    pos(pos),
    name(name),
    namespaces(namespaces),
    definition(definition)
{
}

bool RefClass::operator==(
        const RefClass &other) const
{
    return
            pos == other.pos
            and name == other.name
            and namespaces == other.namespaces
            and definition == other.definition;
}

RefClass::operator QString() const
{
    QStringList tmp = namespaces;
    tmp << name;
    return QString("(%2) %1 %3")
            .arg(tmp.join("::"))
            .arg(pos)
            .arg(definition ? "define" : "");
}
