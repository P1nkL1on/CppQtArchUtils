#ifndef FILEPRO_H
#define FILEPRO_H

#include "file.h"

#include <QMap>

// todo move to separated file, change linnk
// in token_parser

struct ProConfig {
    ProConfig(
            const QString &condition,
            const QString &name,
            const QString &operation,
            const QStringList &values);
    QString condition;
    QString name;
    QString opertation;
    QStringList values;
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
            const QStringList &values);
protected:
    QVector<ProConfig *> m_vars;
};


#endif // FILEPRO_H
