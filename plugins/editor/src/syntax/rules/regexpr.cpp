#include <QRegExp>
#include "regexpr.h"

namespace syntax {

RegExpr::RegExpr(const QString& str, bool insensitive, bool minimal):
    m_regexp(str)
{
    m_regexp.setCaseSensitivity(insensitive ? Qt::CaseInsensitive : Qt::CaseSensitive);
    m_regexp.setMinimal(minimal);
}

MatchResult RegExpr::match(const QString& text, int offset, const QStringList&)
{
    int result = m_regexp.indexIn(text, offset);
    if (result == -1)
        return offset;

    if (result == offset)
        return MatchResult(offset + m_regexp.matchedLength(), m_regexp.capturedTexts());
    return MatchResult(offset, result);
}

}
