#include "file_pro_factory.h"

#include "file_pro.h"
#include "token_parser.h"

Tokenizer FileProFactory::tokenizer() const
{
    return Tokenizer::proTokenizer();
}

File *FileProFactory::createFile(
        const QFileInfo &info,
        const QVector<Token> &tokens) const
{

    QVector<RefFile> refFiles;
    QVector<ProConfig> proConfigs;
    QString guard;
    TokenParser::parsePro(tokens, refFiles, proConfigs);

    File *res = new FilePro(info);
    for (const RefFile &ref : refFiles)
        res->refs.insert(ref, nullptr);

    return res;
}
