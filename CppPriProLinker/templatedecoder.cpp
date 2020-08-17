#include "templatedecoder.h"


FileData TemplateDecoder::decode(const FileData &data) const
{
    // return decode(data, m_variableToTempalteHash, m_variableToStringHash);
}

void TemplateDecoder::addVariableToString(const QString &varName, const QString &stringValue)
{
    m_variableToStringHash.insert(varName, stringValue);
}

void TemplateDecoder::addVariableToTemplate(const QString &varName, const FileData &templateValue)
{
    m_variableToTempalteHash.insert(varName, templateValue);
}

FileData TemplateDecoder::decode(
        const FileData &data,
        const QHash<QString, FileData> &variableToTempalteHash,
        const QHash<QString, QStringList> &variableToStringListHash,
        const QHash<QString, QString> &variableToStringHash)
{
}

void TemplateDecoder::replaceTemplate(FileData &data, const QHash<QString, FileData> &variableToTempalteHash)
{
    for (int i = 0; i < data.length(); ++i){
        const QString line = data.at(i).trimmed();
        const bool isVar = variableToTempalteHash.contains(line);
        if (not isVar)
            continue;
        const FileData replaceData = variableToTempalteHash.value(line);
        if (replaceData.isEmpty()){
            data.removeAt(i);
            --i;
            continue;
        }
        data[i] = replaceData.first();
        for (int j = 1; j < replaceData.length(); ++j)
            data.insert(i + j, data[j]);
        i += replaceData.length() - 1;
    }
}

void TemplateDecoder::replaceLists(FileData &data, const QHash<QString, QStringList> &variableToStringListHash)
{

}

void TemplateDecoder::replaceVars(FileData &data, const QHash<QString, QString> &variableToStringHash)
{
    for (const QString &varName : variableToStringHash.keys()){
        const QString varValue = variableToStringHash.value(varName);
        data.replaceInStrings(varName, varValue);
    }
}
