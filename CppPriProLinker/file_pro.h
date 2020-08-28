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
    QString condition;
    QString name;
    QString opertation;
    QString value;
};

class FilePro : public File
{
public:
    explicit FilePro(const QFileInfo &fileInfo);
    ~FilePro() override;
    void addVar(
            const QString &condition,
            const QString &name,
            const QString &operation,
            const QString &value);
protected:
    QVector<ProConfig *> m_vars;
};


#endif // FILEPRO_H
