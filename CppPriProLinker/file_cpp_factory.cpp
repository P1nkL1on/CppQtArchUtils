#include "file_cpp_factory.h"

#include "file_cpp.h"
#include "token_parser.h"

Tokenizer FileCppFactory::tokenizer() const
{
    return Tokenizer::cppTokenizer();
}

File *FileCppFactory::createFile(const QVector<Token> &tokens) const
{
    QVector<RefFile> rawRefFiles;
    QVector<RefClass> refClasses;
    QString guard;
    TokenParser::parseCpp(tokens, rawRefFiles, refClasses, guard);

    // todo move QFileInfo to results of reading a file
    File *res = new FileCpp();
    res->addRawRefs(rawRefFiles);

    return res;
}
