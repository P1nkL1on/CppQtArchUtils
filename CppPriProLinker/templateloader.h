#ifndef TEMPLATELOADER_H
#define TEMPLATELOADER_H

#include "filedata.h"

#include <QMap>

class TemplateLoader
{
public:
    TemplateLoader(
            const QMap<QString, QString> &templateNameToPathMap = {},
            const QString newLineSeparator = "\n");
    QStringList templateNamesList() const;
    bool readTemplate(const QString &templateName, FileData &data, QString &errMessage) const;
protected:
    QMap<QString, QString> m_templateNameToPathMap;
    QString m_newLineSeparator;
    bool hasTemplate(const QString &templateName) const;
    static bool readFileData(const QString &filePath, const QString &newLineSeparator, FileData &data, QString &errMessage);
    friend class TemplateLoaderTests;
};

#endif // TEMPLATELOADER_H
