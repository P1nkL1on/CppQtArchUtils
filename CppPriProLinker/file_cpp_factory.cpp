#include "file_cpp_factory.h"

#include "file_cpp.h"
#include "token_parser.h"

Tokenizer FileCppFactory::tokenizer() const
{
    return Tokenizer::cppTokenizer();
}

File *FileCppFactory::createFile(
        const QFileInfo &info,
        const QVector<Token> &tokens) const
{
    QVector<RefFile> refFiles;
    QVector<RefClass> refClasses;
    QString guard;
    TokenParser::parseCpp(tokens, refFiles, refClasses, guard);

    // todo move QFileInfo to results of reading a file
    File *res = new FileCpp(info);
    for (const RefFile &ref : refFiles)
        res->refs.insert(ref, nullptr);

    return res;
}
