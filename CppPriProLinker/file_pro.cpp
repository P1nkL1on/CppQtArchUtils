#include "file_pro.h"

void FilePro::addConfig(const ProConfig &config)
{
    m_config << config;
}

ProConfig::ProConfig(
        const QString &condition,
        const QString &name,
        const QString &operation,
        const QString &value):
    condition(condition),
    name(name),
    opertation(operation),
    value(value)
{

}

bool ProConfig::operator==(const ProConfig &other) const
{
    return condition == other.condition
            and name == other.name
            and opertation == other.opertation
            and value == other.value;
}
