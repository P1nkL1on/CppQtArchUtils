#include "file_pro.h"

FilePro::FilePro(const QFileInfo &fileInfo) :
    File(fileInfo)
{
}

FilePro::~FilePro()
{
    qDeleteAll(m_vars);
}

void FilePro::addVar(
        const QString &condition,
        const QString &name,
        const QString &operation,
        const QString &value)
{
    m_vars << new ProConfig(
                  condition,
                  name,
                  operation,
                  value);
}

ProConfig::ProConfig(
        const QString &condition,
        const QString &name,
        const QString &operation,
        const QString &value) :
    condition(condition),
    name(name),
    opertation(operation),
    value(value)
{
    Q_ASSERT(operation.right(1) == "=");
    Q_ASSERT(not name.isEmpty());
    Q_ASSERT(not value.isEmpty());
}
