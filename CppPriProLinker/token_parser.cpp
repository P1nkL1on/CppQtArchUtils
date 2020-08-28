#include "token_parser.h"

#include <QFile>
#include <QFileInfo>
#include <QVector>

#include "file_pro.h"

bool TokenParser::readPlainFileData(
        const QString &filePath,
        PlainFileData &data,
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
    const QByteArray byteData = file.readAll();
    file.close();
    data = QString(byteData);
    errMessage = QString();
    return true;
}

void TokenParser::parseCpp(
        const QVector<Token> &tokens,
        QVector<RefFile> &includes,
        QVector<RefClass> &classes,
        QString &guard)
{
    const QVector<CppTokenType> skipTokens {
        CppTokenType::AreaComment,
        CppTokenType::LineComment,
        CppTokenType::Qoute,
        CppTokenType::Char
    };
    enum SaveNextTokenAs {
        None, Namespace, Class, Guard
    };
    const QStringList blockIdentifiers {"namespace", "class", "struct", "enum"};
    QStringList currentBlockStack;
    QString lastBlockName;
    int currentBlockDepth = 0;
    SaveNextTokenAs saveNextAs = None;

    for (const Token &token : tokens)
        switch (token.type) {
        case int(CppTokenType::SemiColon):
            lastBlockName.clear();
            continue;
        case int(CppTokenType::CurlyOpen):
            if (not lastBlockName.isEmpty())
                currentBlockStack << lastBlockName;
            lastBlockName.clear();
            ++currentBlockDepth;
            continue;
        case int(CppTokenType::CurlyClose):
            if (currentBlockDepth < currentBlockStack.length())
                currentBlockStack.removeLast();
            --currentBlockDepth;
            continue;
        case int(CppTokenType::Include):
            includes << includeTokenToRawRef(token);
            continue;
        case int(CppTokenType::Directive):
            if (saveNextAs == None and token.text == "#ifndef")
                saveNextAs = Guard;
            continue;
        case int(CppTokenType::Identifer):
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
            if (saveNextAs == Class)
                classes << nameTokenToRawRef(currentBlockStack, token);
            saveNextAs = None;
            continue;
        default:
            continue;
        }
}

RefFile TokenParser::includeTokenToRawRef(
        const Token &token)
{
    Q_ASSERT(token.type == int(CppTokenType::Include));
    const QString linkWithBracketsText = token.text.split(' ', QString::SkipEmptyParts).last();
    const QString linkText = linkWithBracketsText.mid(1, linkWithBracketsText.length() - 2);
    const bool isSystem = not linkWithBracketsText.startsWith("\"");
    return RefFile(token.pos, linkText, isSystem);
}

RefClass TokenParser::nameTokenToRawRef(
        const QStringList &currentBlockStack,
        const Token &token)
{
    QStringList nameScopes = currentBlockStack;
    nameScopes << token.text.split("::");
    const QString name = nameScopes.takeLast();
    return RefClass(token.pos + token.text.indexOf(name), name, nameScopes, true);
}

void TokenParser::parsePro(
        const FileData &data,
        QVector<RefFile> &links)
{

}
