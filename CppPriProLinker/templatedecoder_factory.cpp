#include "templatedecoder_factory.h"

#include <QtDebug>

TemplateDecoder TemplateDecoderFactory::create(
        const QStringList &priRelativePathes,
        const QStringList &headerRelativePathes,
        const QStringList &sourceRelativePathes,
        const QStringList &resourceRelativePathes,
        int tabSpaceCount,
        bool skipEmptyBlocks,
        int newLinesBetweenBlocks,
        bool usePriPWD,
        bool useCppPWD)
{
    TemplateDecoder decoder;

    // FILE BLOCKS
    const QHash<QString, QStringList> blockNameToFileNames = {
        {"HS", headerRelativePathes},
        {"CPPS", sourceRelativePathes},
        {"QRCS", resourceRelativePathes}
    };
    const QHash<QString, QString> blockNameToBlockKey = {
        {"HS", "HEADERS"},
        {"CPPS", "SOURCES"},
        {"QRCS", "RESOURCES"}
    };

    const auto addFileBlock = [&](const QString &blockName, const QString &blockTitle, const QStringList &blockFileRelativePathes){
        if (blockFileRelativePathes.isEmpty() and skipEmptyBlocks){
            decoder.setVariableToTemplate("$" + blockName, QStringList());
            return;
        }
        const QString pwdStr = useCppPWD ? "$$PWD/" : "";
        QStringList blockTemplateData {
            QString("%1$%2_NEWLINE_START")
                    .arg(blockTitle).arg(blockName),
            "$REPEAT",
            QString("\t%2$%1_RELATIVE_PATHES$%1_LINE_SEPARATORS")
                    .arg(blockName).arg(pwdStr),
            "$END"
        };
        for (int i = 0; i < newLinesBetweenBlocks; ++i)
            blockTemplateData.insert(0, "");
        const int size = blockFileRelativePathes.length();
        QStringList lineSeparators;
        for (int i = 0; i < size; ++i, lineSeparators << (i < size ? " \\" : ""));

        decoder.setVariableToTemplate("$" + blockName, blockTemplateData);
        decoder.setVariableToStringList(QString("$%1_RELATIVE_PATHES").arg(blockName), blockFileRelativePathes);
        decoder.setVariableToStringList(QString("$%1_LINE_SEPARATORS").arg(blockName), lineSeparators);
        decoder.setVariableToString(QString("$%1_NEWLINE_START").arg(blockName), blockFileRelativePathes.isEmpty() ? "" : " += \\");
    };

    for (const QString &blockName : blockNameToFileNames.keys()){
        const QStringList blockFilePathes = blockNameToFileNames.value(blockName);
        addFileBlock(blockName, blockNameToBlockKey.value(blockName), blockFilePathes);
    }

    // PRIS
    const auto addPris = [&](const QStringList &priRelativePathes){
        if (priRelativePathes.isEmpty() and skipEmptyBlocks){
            decoder.setVariableToTemplate("$PRIS", QStringList());
            return;
        }
        const QString pwdStr = usePriPWD ? "$$PWD/" : "";
        QStringList priTemplate {
            "$REPEAT",
            QString("include(%1$PRI_RELATIVE_PATHES)").arg(pwdStr),
            "$END"
        };
        for (int i = 0; i < newLinesBetweenBlocks; ++i)
            priTemplate.insert(0, "");
        decoder.setVariableToTemplate("$PRIS", priTemplate);
        decoder.setVariableToStringList("$PRI_RELATIVE_PATHES", priRelativePathes);
    };
    addPris(priRelativePathes);

    // MISC OPTIONS
    // todo move it to the inner method of decoder
    // because now it can replaces tabs in the middle and end of line
    // but only at the line start its is required
    decoder.setVariableToString("\t", QString().leftJustified(tabSpaceCount, ' '));

    return decoder;
}
