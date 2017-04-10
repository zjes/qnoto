#include <QRegExp>
#include <QDebug>
#include "regexpr.h"

namespace syntax {

RegExpr::RegExpr(const QString& str, bool insensitive, bool minimal):
    m_regexp(str)
{
    m_regexp.setCaseSensitivity(insensitive ? Qt::CaseInsensitive : Qt::CaseSensitive);
    m_regexp.setMinimal(minimal);
    if (!m_regexp.isValid()){
        qWarning() << "Regexp ! valid" << m_regexp.errorString();
    }
}

int RegExpr::match(const QString& text, int offset)
{
    int result = m_regexp.indexIn(text, offset);
    if (result != offset)
        return offset;

    if (result == offset)
        return offset + m_regexp.matchedLength();

    return offset;
}

}
