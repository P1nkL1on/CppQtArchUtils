#include "file_pro_factory.h"

#include "file_pro.h"
#include "token_parser.h"

Tokenizer FileProFactory::tokenizer() const
{
    return Tokenizer::proTokenizer();
}

File *FileProFactory::createFile(const QVector<Token> &tokens) const
{

    QVector<RefFile> rawRefFiles;
    QVector<ProConfig> proConfigs;
    QString guard;
    TokenParser::parsePro(tokens, rawRefFiles, proConfigs);

    File *res = new FilePro();
    res->addRawRefs(rawRefFiles);
    return res;
}
