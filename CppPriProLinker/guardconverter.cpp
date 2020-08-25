#include "guardconverter.h"


enum LetterType {
    Other = -1,
    Upper,
    Lower,
    Digit
};

QString GuardcCnverter::convert(const QString &name)
{
    if (name.isEmpty())
        return QString();
    int letterInd = 0;
    LetterType type = Other;
    QString res;
    const int nameLength = name.length();
    do {
        const QChar letter = name.at(letterInd);
        const LetterType currentType =
                letter.isDigit() ? Digit
              : letter.isLower() ? Lower
              : letter.isUpper() ? Upper
                                 : Other;
        const bool useUnderScore =
                not res.isEmpty()
                and (currentType != type)
                and not (type == Upper and currentType == Lower);
        const bool skip =
                currentType == Other
                or (useUnderScore and not res.isEmpty() and res.at(res.size() - 1) == "_");
        if (not skip and useUnderScore)
            res += "_";
        if (not skip)
            res += letter.toUpper();

        type = currentType;
        ++letterInd;
    } while (letterInd < nameLength);
    return res;
}
