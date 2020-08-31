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


const QStringList blockIdentifiers {
    "namespace", "class", "struct", "enum"
};
const QStringList cppKeyWords {
    "asm", "auto", "bool", "break", "case",
    "catch", "char", "class", "const",
    "continue", "default", "delete", "do",
    "double", "else", "enum", "explicit",
    "export", "extern", "false", "float", "for",
    "friend", "goto", "if", "inline", "int",
    "long", "mutable", "namespace", "new", "operator",
    "private", "protected," "public", "register",
    "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "template", "this",
    "throw", "true", "try", "typedef", "typename",
    "union", "unsigned", "using", "virtual", "void",
    "volatile", "wchar_t", "while", "and", "or", "not"
};

void TokenParser::parseCpp(
        const QVector<Token> &tokens,
        QVector<RefFile> &includes,
        QVector<RefClass> &classes,
        QString &guard)
{
    // todo add remember deepest namespacing
    // and trat it as a namespace for no-class contains files
    // ex: any utils with funcs in namespaces

    // separate exact namespaces (when defining entities)
    // and maybenamespaces in all other cases

    const QVector<CppTokenType> skipTokens {
        CppTokenType::AreaComment,
                CppTokenType::LineComment,
                CppTokenType::Qoute,
                CppTokenType::Char
    };
    enum SaveNextTokenAs {
        None = -1, Namespace, Class, Guard
    };
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
            if (saveNextAs == None){
                continue;
            }
            if (saveNextAs == Guard){
                guard = token.text;
                saveNextAs = None;
                continue;
            }
            lastBlockName = token.text;
            if (saveNextAs == Class)
                classes << classTokenToRawRef(currentBlockStack, token);
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
    return RefFile(token.pos + token.text.indexOf(linkText), linkText, isSystem);
}

RefClass TokenParser::classTokenToRawRef(
        const QStringList &currentBlockStack,
        const Token &token)
{
    QStringList nameScopes = currentBlockStack;
    nameScopes << token.text.split("::");
    const QString name = nameScopes.takeLast();
    const int pos = token.pos + token.text.indexOf(name);
    return RefClass(pos, name, nameScopes);
}

void TokenParser::parsePro(
        const QVector<Token> &tokens,
        QVector<RefFile> &links,
        QVector<ProConfig> &configs)
{
    enum SaveNextTokenAs {
        None = -1, Headers, Sources, Resources, OtherFiles, Include
    };
    const QStringList listBlockNames {
        "HEADERS", "SOURCES", "RESOURCES", "OTHERFILES", "include"
    };
    SaveNextTokenAs saveNextAs = None;

    for (const Token &token : tokens)
        switch (token.type) {
        case int(ProTokenType::LineBreak):
            saveNextAs = None;
            continue;
        case int(ProTokenType::Operator):
            continue;
        case int(ProTokenType::Identifer):
            switch (saveNextAs) {
            case None:
                saveNextAs = SaveNextTokenAs(listBlockNames.indexOf(token.text));
                continue;
            case Include:
                links << RefFile(token.pos, token.text);
                saveNextAs = None;
                continue;
            default:
                links << RefFile(token.pos, token.text);
                continue;
            }
        default:
            continue;
        }
}
