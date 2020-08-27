#ifndef TEMPLATELOADER_H
#define TEMPLATELOADER_H

#include "token_parser.h"

#include <QMap>

class TemplateLoader
{
public:
    TemplateLoader(
            const QMap<QString, QString> &templateNameToPathMap = {});
    QStringList templateNamesList() const;
    bool readTemplate(const QString &templateName, FileData &data, QString &errMessage) const;
protected:
    QMap<QString, QString> m_templateNameToPathMap;
    bool hasTemplate(const QString &templateName) const;
    friend class TemplateLoaderTests;
};

#endif // TEMPLATELOADER_H