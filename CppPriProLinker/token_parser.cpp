#include "token_parser.h"

#include <QFile>
#include <QFileInfo>
#include <QVector>

bool TokenParser::readFileData(
        const QString &filePath,
        FileData &data,
        QString &errMessage)
{
    QFileInfo info(filePath);
    if (not info.exists()){
        errMessage = QString("File doen't exist! %1").arg(filePath);
        return false;
    }

    QFile file(filePath);
    if (not file.open(QIODevice::ReadOnly)){
        errMessage = QString("File can't be readen! %1").arg(filePath);
        return false;
    }
    while (not file.atEnd())
        data << QString(file.readLine()).replace("\r\n", "");
    file.close();
    errMessage = QString();
    return true;
}

void TokenParser::parseCpp(
        const QVector<Token> &tokens,
        QStringList &includes,
        QStringList &namespaces,
        QStringList &classes,
        QString &guard)
{
    const QVector<TokenType> skipTokens {
        AreaComment, LineComment, Qoute, Char
    };
    const QRegExp spaceRegExp("(\\s|\\{)+");
    QStringList currentNamespaces;
    int currentBlockDepth = 0;
    for (const Token &token : tokens){
        if (skipTokens.contains(token.type))
            continue;
        if (token.type == BlockOpen){
            ++currentBlockDepth;
            continue;
        }
        if (token.type == BlockClose){
            if (currentBlockDepth < currentNamespaces.length())
                currentNamespaces.removeLast();
            --currentBlockDepth;
            continue;
        }
        const QStringList splitedToken = token.text.trimmed()
                .split(spaceRegExp, QString::SkipEmptyParts);
        const QString value = splitedToken.size() > 1 ?
                    splitedToken[1] : QString();
        if (token.type == Directive){
            if (splitedToken.first() == "#include")
                includes << value;
            else if (guard.isEmpty() and splitedToken.first() == "#ifndef")
                guard = value;
        } else if (token.type == StructBlockOpen){
            currentNamespaces << value;
            ++currentBlockDepth;
            if (not namespaces.contains(value) and splitedToken.first() == "namespace"){
                namespaces << value;
            } else {
                classes << currentNamespaces.join("::");
            }
        }
    }
}

void TokenParser::parsePri(
        const FileData &data,
        QVector<FileLink> &links)
{

}
