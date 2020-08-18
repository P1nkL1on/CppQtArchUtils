#include "templatedecoder.h"

FileData TemplateDecoder::decode(const FileData &data) const
{
    return decode(
        data,
        m_variableToTempalteHash,
        m_variableToStringListHash,
        m_variableToStringHash);
}

void TemplateDecoder::setVariableToString(const QString &varName, const QString &stringValue)
{
    m_variableToStringHash.insert(varName, stringValue);
}

void TemplateDecoder::setVariableToStringList(const QString &varName, const QStringList &stringListValue)
{
    m_variableToStringListHash.insert(varName, stringListValue);
}

void TemplateDecoder::setVariableToTemplate(const QString &varName, const FileData &templateValue)
{
    m_variableToTempalteHash.insert(varName, templateValue);
}

FileData TemplateDecoder::decode(
        const FileData &data,
        const QHash<QString, FileData> &variableToTempalteHash,
        const QHash<QString, QStringList> &variableToStringListHash,
        const QHash<QString, QString> &variableToStringHash)
{
    FileData res = data;
    replaceTemplate(res, variableToTempalteHash);
    replaceLists(res, variableToStringListHash);
    replaceVars(res, variableToStringHash);
    return res;
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
            data.insert(i + j, replaceData[j]);
        i += replaceData.length() - 1;
    }
}

void TemplateDecoder::replaceLists(FileData &data, const QHash<QString, QStringList> &variableToStringListHash)
{
    int startLineInd = -1;
    int endLineInd = -1;
    bool isRepeatBackward = false;
    bool isInLineBuffer = false;
    QStringList linesBuffer;
    QStringList variableNamesInBuffer;
    const QStringList variableNames = variableToStringListHash.keys();

    const auto replaceBuffer = [&]{
        Q_ASSERT_X(not variableNamesInBuffer.isEmpty(), "TemplateDecoder::replaceLists",
                  "No variables inside repeat cycle means no way to compute iterations count!");
        const int oldBufferLength = endLineInd - startLineInd + 1;
        for (int j = 0; j < oldBufferLength; ++j)
            data.removeAt(startLineInd);

        int cycleLength = -1;
        for (const QString &variableNameInBuffer : variableNamesInBuffer){
            const int tmp = variableToStringListHash.value(variableNameInBuffer).length();
            Q_ASSERT_X(tmp == cycleLength or cycleLength < 0, "TemplateDecoder::replaceLists",
                       "Var lists inside one repeat cycle should contain same amount of values!");
            cycleLength = tmp;
        }

        int insertInd = startLineInd;
        for (int cycleInd = 0; cycleInd < cycleLength; ++cycleInd){
            for (const QString &bufferedLine : linesBuffer){
                QString tmp = bufferedLine;
                for (const QString &variableNameInBuffer : variableNamesInBuffer){
                    const int valueInd = isRepeatBackward ? (cycleLength - 1 - cycleInd) : cycleInd;
                    tmp.replace(variableNameInBuffer, variableToStringListHash
                                .value(variableNameInBuffer).at(valueInd));
                }
                data.insert(insertInd, tmp);
                ++insertInd;
            }
        }
        const int newBufferLength = cycleLength * linesBuffer.length();
        return newBufferLength - oldBufferLength;
    };

    for (int i = 0; i < data.length(); ++i) {
        const QString line = data.at(i);
        const QString lineTrimmed = line.trimmed();
        if (isInLineBuffer){
            if (lineTrimmed != "$END"){
                linesBuffer << line;
                for (const QString &variableName : variableNames)
                    if (line.contains(variableName) and not variableNamesInBuffer.contains(variableName))
                        variableNamesInBuffer << variableName;
                continue;
            }
            endLineInd = i;
            const int offset = replaceBuffer();
            i += offset;

            startLineInd = -1;
            endLineInd = -1;
            isInLineBuffer = false;
            linesBuffer.clear();
            variableNamesInBuffer.clear();
            continue;
        }
        if (lineTrimmed.startsWith("$REPEAT")){
            isInLineBuffer = true;
            isRepeatBackward = lineTrimmed == "$REPEAT_BACKWARD";
            startLineInd = i;
            continue;
        }
    }
}

void TemplateDecoder::replaceVars(FileData &data, const QHash<QString, QString> &variableToStringHash)
{
    for (const QString &varName : variableToStringHash.keys()){
        const QString varValue = variableToStringHash.value(varName);
        data.replaceInStrings(varName, varValue);
    }
}
