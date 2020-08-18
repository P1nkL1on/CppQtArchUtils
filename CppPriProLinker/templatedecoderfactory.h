#ifndef CPPTEMPLATEDECODERFACTORY_H
#define CPPTEMPLATEDECODERFACTORY_H

#include "templatedecoder.h"

class TemplateDecoderFactory
{
public:
    static TemplateDecoder create(
            const QStringList &priRelativePathes,
            const QStringList &headerRelativePathes,
            const QStringList &sourceRelativePathes,
            const QStringList &resourceRelativePathes,
            int tabSpaceCount,
            bool skipEmptyBlocks,
            int newLinesBetweenBlocks);
};

#endif // CPPTEMPLATEDECODERFACTORY_H
