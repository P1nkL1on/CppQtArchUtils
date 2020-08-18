#include "templatedecoderfactory.h"

#include <QtDebug>

TemplateDecoder TemplateDecoderFactory::create(
        const QStringList &priRelativePathes,
        const QStringList &headerRelativePathes,
        const QStringList &sourceRelativePathes,
        const QStringList &resourceRelativePathes,
        bool skipEmptyBlocks,
        int newLinesBetweenBlocks)
{
    TemplateDecoder decoder;

    // FILE BLOCKS
    const QHash<QString, QStringList> blockNameToFileNames = {
        {"HEADERS", headerRelativePathes},
        {"SOURCES", sourceRelativePathes},
        {"RESOURCES", resourceRelativePathes}
    };
    const auto blockTemplate = [](const QString &name) -> QStringList {
        return QString("%1 += \\ | $REPEAT | \t$%1_RELATIVE_PATHES$%1_LINE_SEPARATORS | $END")
                .arg(name).split(" | ");
    };

    const auto addFileBlock = [&](const QString &blockName, const QStringList &blockFileRelativePathes){
        if (blockFileRelativePathes.isEmpty() and skipEmptyBlocks){
            decoder.setVariableToTemplate("$" + blockName, QStringList());
            return;
        }
        const int size = blockFileRelativePathes.length();
        QStringList lineSeparators;
        for (int i = 0; i < size; ++i, lineSeparators << (i < size ? " \\" : ""));

        QStringList blockTemplate = blockTemplate(blockName);
        for (int i = 0; i < newLinesBetweenBlocks; ++i) blockTemplate.insert(0, "");
        decoder.setVariableToTemplate("$" + blockName, blockTemplate);
        decoder.setVariableToStringList(QString("%1_RELATIVE_PATHES").arg(blockName), blockFileRelativePathes);
        decoder.setVariableToStringList(QString("%1_LINE_SEPARATORS").arg(blockName), lineSeparators);
    };

    for (const QString &blockName : blockNameToFileNames.keys()){
        const QStringList blockFilePathes = blockNameToFileNames.value(blockName);
        addFileBlock(blockName, blockFilePathes);
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

    return decoder;
}
