#include "template_loader.h"
#include <QtDebug>

TemplateLoader::TemplateLoader(const QMap<QString, QString> &templateNameToPathMap) :
    m_templateNameToPathMap(templateNameToPathMap)
{
}

QStringList TemplateLoader::templateNamesList() const
{
    return m_templateNameToPathMap.keys();
}

bool TemplateLoader::readTemplate(const QString &templateName, FileData &data, QString &errMessage) const
{
    Q_ASSERT(hasTemplate(templateName));
    const QString filePath = m_templateNameToPathMap.value(templateName);
    return FileParser::readFileData(filePath, data, errMessage);
}

bool TemplateLoader::hasTemplate(const QString &templateName) const
{
    return m_templateNameToPathMap.contains(templateName);
}
