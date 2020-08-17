#ifndef TEMPLATEDECODER_H
#define TEMPLATEDECODER_H

#include "filedata.h"

#include <QHash>

// var --> string
// $NAME --> "MyTestClass"

// var --> stringList
// $NAMESPACES --> {"Project", "App", "Module", "Tests"}

// var --> template
// 1. $NAMESPACE_START -->

// QStringList {
// $REPEAT
// $namespace $NAMESPACES {
// $_
// $END
// }
//
// 2. $NAMESPACE_END -->

// QStringList {
// $REPEAT_BACKWARD
// $_
// $} // NAMESPACES
// }

class TemplateDecoder
{
public:
    TemplateDecoder() = default;
    FileData decode(const FileData &data) const;
    void addVariableToString(const QString &varName, const QString &stringValue);
    void addVariableToStringList(const QString &varName, const QStringList &stringListValue);
    void addVariableToTemplate(const QString &varName, const FileData &templateValue);
protected:
    QHash<QString, FileData> m_variableToTempalteHash;
    QHash<QString, QString> m_variableToStringHash;
    static FileData decode(
            const FileData &data,
            const QHash<QString, FileData> &variableToTempalteHash,
            const QHash<QString, QStringList> &variableToStringListHash,
            const QHash<QString, QString> &variableToStringHash);
    static void replaceTemplate(FileData &data, const QHash<QString, FileData> &variableToTempalteHash);
    static void replaceLists(FileData &data, const QHash<QString, QStringList> &variableToStringListHash);
    static void replaceVars(FileData &data, const QHash<QString, QString> &variableToStringHash);
    friend class TemplateDecoderTests;
};

#endif // TEMPLATEDECODER_H