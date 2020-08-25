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
            int tabSpaceCount = 4,
            bool skipEmptyBlocks = true,
            int newLinesBetweenBlocks = 1,
            bool usePriPWD = true,
            bool useCppPWD = false);
};

#endif // CPPTEMPLATEDECODERFACTORY_H
