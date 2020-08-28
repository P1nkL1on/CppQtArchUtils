#include "file_pro_factory.h"

#include "file_pro.h"
#include "token_parser.h"
#include "tokenizer.h"

File *FileProFactory::read(const QString &filePath, QString &err)
{
    // todo move common part to parent class
    // common with file_cpp_factory and other siblings
    PlainFileData data;
    if (not TokenParser::readPlainFileData(filePath, data, err))
        return nullptr;
    const Tokenizer tokenizer = Tokenizer::headerTokenizer();
    const QVector<Token> tokens = tokenizer.tokenize(data);

    QVector<RefFile> refFiles;
    QVector<ProConfig> proConfigs;
    QString guard;
    TokenParser::parsePro(tokens, refFiles, proConfigs);

    const QFileInfo fileInfo(filePath);
    File *res = new File(fileInfo);
    for (const RefFile &ref : refFiles)
        res->refs.insert(ref, nullptr);

    return res;
}
