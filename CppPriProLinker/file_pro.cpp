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
        const QStringList &values)
{
    m_vars << new ProConfig(
                  condition,
                  name,
                  operation,
                  values);
}

ProConfig::ProConfig(
        const QString &condition,
        const QString &name,
        const QString &operation,
        const QStringList &values) :
    condition(condition),
    name(name),
    opertation(operation),
    values(values)
{
    Q_ASSERT(operation.right(1) == "=");
    Q_ASSERT(not name.isEmpty());
    Q_ASSERT(not values.isEmpty());
}
