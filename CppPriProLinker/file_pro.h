#ifndef FILEPRO_H
#define FILEPRO_H

#include "file.h"

#include <QMap>

// todo move to separated file, change linnk
// in token_parser

struct ProConfig {
    ProConfig() = default;
    ProConfig(
            const QString &condition,
            const QString &name,
            const QString &operation,
            const QString &value);
    bool operator== (
            const ProConfig &other) const;
    QString condition;
    QString name;
    QString opertation;
    QString value;
};

class FilePro : public File
{
public:
    explicit FilePro() = default;
    void addConfig(const ProConfig &config);
protected:
    QVector<ProConfig> m_config;
};


#endif // FILEPRO_H
