#include "file_cpp_factory.h"

#include "file_cpp.h"
#include "token_parser.h"

File *FileCppFactory::read(const QString &filePath, QString &err)
{
    PlainFileData data;
    if (not TokenParser::readPlainFileData(filePath, data, err))
        return nullptr;
    const QVector<Token> tokens = m_tokenizer.tokenize(data);

    QVector<RefFile> refFiles;
    QVector<RefClass> refClasses;
    QString guard;
    TokenParser::parseCpp(tokens, refFiles, refClasses, guard);

    // todo move QFileInfo to results of reading a file
    const QFileInfo fileInfo(filePath);
    File *res = new File(fileInfo);
    for (const RefFile &ref : refFiles)
        res->refs.insert(ref, nullptr);

    return res;
}
