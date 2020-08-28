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
        QStringList &classes,
        QString &guard)
{
    const QVector<TokenType> skipTokens {
        TokenType::AreaComment,
        TokenType::LineComment,
        TokenType::Qoute,
        TokenType::Char
    };
    enum SaveNextTokenAs {
        None, Namespace, Class, Guard
    };
    const QRegExp spaceRegExp("(\\s|\\{)+");
    const QStringList blockIdentifiers {"namespace", "class", "struct", "enum"};
    QStringList currentBlockStack;
    QString lastBlockName;
    int currentBlockDepth = 0;
    SaveNextTokenAs saveNextAs = None;

    for (const Token &token : tokens)
        switch (token.type) {
        case int(TokenType::SemiColon):
            lastBlockName.clear();
            continue;
        case int(TokenType::CurlyOpen):
            if (not lastBlockName.isEmpty())
                currentBlockStack << lastBlockName;
            lastBlockName.clear();
            ++currentBlockDepth;
            continue;
        case int(TokenType::CurlyClose):
            if (currentBlockDepth < currentBlockStack.length())
                currentBlockStack.removeLast();
            --currentBlockDepth;
            continue;
        case int(TokenType::Include):
            includes << token.text.split(spaceRegExp, QString::SkipEmptyParts).last();
            continue;
        case int(TokenType::Directive):
            if (saveNextAs == None and token.text == "#ifndef")
                saveNextAs = Guard;
            continue;
        case int(TokenType::Identifer):
            if (blockIdentifiers.contains(token.text)){
                saveNextAs = blockIdentifiers.indexOf(token.text) ? Class : Namespace;
                continue;
            }
            if (saveNextAs == None)
                continue;
            if (saveNextAs == Guard){
                guard = token.text;
                saveNextAs = None;
                continue;
            }
            lastBlockName = token.text;
            if (saveNextAs == Class){
                QStringList fullBlockStack = currentBlockStack;
                fullBlockStack << token.text;
                classes << fullBlockStack.join("::");
            }
            saveNextAs = None;
            continue;
        default:
            continue;
        }
}

void TokenParser::parsePri(
        const FileData &data,
        QVector<FileLink> &links)
{

}
