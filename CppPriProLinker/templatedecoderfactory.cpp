#include "templatedecoderfactory.h"

#include <QtDebug>

TemplateDecoder TemplateDecoderFactory::create(
        const QStringList &priRelativePathes,
        const QStringList &headerRelativePathes,
        const QStringList &sourceRelativePathes,
        const QStringList &resourceRelativePathes,
        int tabSpaceCount,
        bool skipEmptyBlocks,
        int newLinesBetweenBlocks)
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
    const auto blockTemplate = [](const QString &title, const QString &name) -> QStringList {
        return QString("%2 += \\ | $REPEAT | \t$%1_RELATIVE_PATHES$%1_LINE_SEPARATORS | $END")
                .arg(name).arg(title).split(" | ");
    };

    const auto addFileBlock = [&](const QString &blockName, const QString &blockTitle, const QStringList &blockFileRelativePathes){
        if (blockFileRelativePathes.isEmpty() and skipEmptyBlocks){
            decoder.setVariableToTemplate("$" + blockName, QStringList());
            return;
        }
        const int size = blockFileRelativePathes.length();
        QStringList lineSeparators;
        for (int i = 0; i < size; ++i, lineSeparators << (i < size ? " \\" : ""));

        QStringList blockTemplateData = blockTemplate(blockTitle, blockName);
        for (int i = 0; i < newLinesBetweenBlocks; ++i)
            blockTemplateData.insert(0, "");
        decoder.setVariableToTemplate("$" + blockName, blockTemplateData);
        decoder.setVariableToStringList(QString("$%1_RELATIVE_PATHES").arg(blockName), blockFileRelativePathes);
        decoder.setVariableToStringList(QString("$%1_LINE_SEPARATORS").arg(blockName), lineSeparators);
    };

    for (const QString &blockName : blockNameToFileNames.keys()){
        const QStringList blockFilePathes = blockNameToFileNames.value(blockName);
        addFileBlock(blockName, blockNameToBlockKey.value(blockName), blockFilePathes);
    }

    // PRIS
    const auto addPris = [&](const QStringList &priRelativePathes){
        const QStringList priTemplate {
            "$REPEAT",
            "include($PRI_RELATIVE_PATHES)",
            "$END"
        };
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
