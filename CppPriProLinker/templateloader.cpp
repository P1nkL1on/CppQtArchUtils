#include "templateloader.h"
#include <QFile>
#include <QFileInfo>
#include <QtDebug>

TemplateLoader::TemplateLoader(
        const QMap<QString, QString> &templateNameToPathMap,
        const QString newLineSeparator) :
    m_templateNameToPathMap(templateNameToPathMap),
    m_newLineSeparator(newLineSeparator)
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
    return readFileData(filePath, m_newLineSeparator, data, errMessage);
}

bool TemplateLoader::hasTemplate(const QString &templateName) const
{
    return m_templateNameToPathMap.contains(templateName);
}

bool TemplateLoader::readFileData(const QString &filePath, const QString &newLineSeparator, FileData &data, QString &errMessage)
{
    QFileInfo info(filePath);
    if (not info.exists()){
        errMessage = QString("File doen't exist! %1").arg(filePath);
        return false;
    }

    QFile file(filePath);
    if (not file.open(QIODevice::ReadOnly)){
        errMessage = QString("File can't be readen! %1").arg(filePath);
        return false;
    }
    const QByteArray bytes = file.readAll();
    file.close();

    const QString stringData(bytes);

    Q_ASSERT(not newLineSeparator.isEmpty());

    data = stringData.isEmpty() ?
                QStringList()
              : stringData.split(newLineSeparator);
    errMessage = QString();
    return true;
}
