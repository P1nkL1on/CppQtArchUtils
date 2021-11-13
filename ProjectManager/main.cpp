#include <QCoreApplication>

#include "filetask.h"

struct PriRef
{
    inline bool isResolved() const
    {
        return id.has_value();
    }
    /// source code text
    QString rawText;
    /// parsed into the absFilePath
    QString absFilePath;
    /// id is nullout - no resolving was performed
    /// id is -1 - resolved, but failed to find file, etc
    std::optional<int> id = std::nullopt;
};

namespace Utils {

template <typename T, typename Predicate>
inline auto anyOf(const QVector<T> &vector, Predicate p)
{
    return std::any_of(vector.begin(), vector.end(), std::forward<Predicate>(p));
}

enum class ProTokenType
{
    None = -1,
    Comment = 0,
    Condition,
    CurlyOpen,
    CurlyClose,
    Operator,
    LineContinue,
    LineBreak,
    Identifer,
};

const QVector<QRegExp> proTokensRegExpsVector {
    /*Comment       */ QRegExp("#[^\n]*"),
    /*Condition     */ QRegExp("[\\w .,!:\\(\\)]+(:|(\\s*(?=\\{)))"),
    /*CurlyOpen     */ QRegExp("\\{"),
    /*CurlyClose    */ QRegExp("\\}"),
    /*Operator      */ QRegExp("([-+*\\\\/=]?=)"),
    /*LineContinue  */ QRegExp("\\\\(\\s*)\\n"),    /// when occuired "\" transition to the next line
    /*LineBreak     */ QRegExp("((\\n)+)"),
    /*Identifer     */ QRegExp("([a-zA-Z0-9_\\/\\$\\.\\{\\}-+=]+)"),
};

struct Token
{
    Token() = default;
    Token(const int pos, const int type, const QString &text) :
        pos(pos),
        type(type),
        text(text)
    {
    }
    int pos = -1;
    int type = -1;
    QString text;
};

QDebug operator<<(QDebug debug, const Token &token)
{
    debug << QString("%3: token %1 - %2").arg(token.type).arg(token.text).arg(token.pos);
    return debug.space();
}

/// tests required, you know?
QVector<Token> tokenize(
        const QVector<QRegExp> &tokenRegExps,
        const QString &fileTextJoined)
{
    int currentPos = 0;
    QVector<Token> res;
    QVector<int> cacheTokenPos(tokenRegExps.size(), -1);
    while (currentPos < fileTextJoined.size()){
        int nextInd= -1;
        int minNextPos = fileTextJoined.size();
        for (int typeInd = 0; typeInd < tokenRegExps.size(); ++typeInd){
            const QRegExp &reg = tokenRegExps[typeInd];
            int nextPos = cacheTokenPos[typeInd];
            if (nextPos < currentPos){
                nextPos = reg.indexIn(fileTextJoined, currentPos);
                cacheTokenPos[typeInd] = nextPos;
            }
            if (nextPos >= currentPos and nextPos < minNextPos){
                minNextPos = nextPos;
                nextInd = typeInd;
            }
            if (nextPos == currentPos)
                break;
        }
        if (nextInd == -1)
            break;
        const int length = tokenRegExps[int(nextInd)].matchedLength();
        const QString tokenText = fileTextJoined.mid(minNextPos, length);
        res << Token(minNextPos, nextInd, tokenText);
        currentPos = minNextPos + length;
    }
    return res;
}

}

class Pri
{
public:
    inline bool hasUnresovedRefs() const
    {
        return Utils::anyOf(m_refs, [](const PriRef &priRef){
            return !priRef.isResolved();
        });
    }

//private:
    const int m_id = -1;
    const QString m_filePathOrig;
    QVector<PriRef> m_refs;
};

class PriGraph
{
public:
    /// root is meant the pri, which includes will be
    /// parsed and stored in m_pris member
    PriGraph(const QString &absFilePathRootPri)
    {

    }
    inline bool hasUnresovedPriRefs() const
    {
        return Utils::anyOf(m_pris, [](const Pri &pri){
            return pri.hasUnresovedRefs();
        });
    }
    QVector<Pri> m_pris;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("/home/alex/jff/CppQtArchUtils/CppPriProLinker/CppPriProLinker.pro");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    const QString text = file.readAll();

    for (const Utils::Token &t : Utils::tokenize(Utils::proTokensRegExpsVector, text))
        qDebug() << t;

    return a.exec();
}
