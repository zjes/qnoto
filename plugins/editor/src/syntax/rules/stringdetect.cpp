#include "stringdetect.h"

namespace syntax {

StringDetect::StringDetect(const QString& str, bool insensitive):
    m_string(str),
    m_insensitive(insensitive ? Qt::CaseInsensitive: Qt::CaseSensitive)
{}

MatchResult StringDetect::match(const QString& text, int offset, const QStringList&)
{
    if (text.midRef(offset, m_string.size()).compare(m_string, m_insensitive) == 0)
        return offset + m_string.size();
    return offset;
}

}
