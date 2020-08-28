#include "ref_file.h"
#include "file.h"

RefFile::RefFile(
        const int pos,
        const QString &text,
        bool isSystem) :
    pos(pos),
    text(text),
    isSystem(isSystem)
{
}

bool RefFile::operator==(
        const RefFile &other) const
{
    return
            pos == other.pos
            and text == other.text
            and isSystem == other.isSystem;
}

RefFile::operator QString() const
{
    return QString("%3(%2) %1%3")
            .arg(text)
            .arg(pos)
            .arg(isSystem ? "sys " : "");
}
