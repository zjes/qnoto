#include "detect2char.h"

namespace syntax {

Detect2Char::Detect2Char(const QChar& ch, const QChar& ch1):
    m_char(ch),
    m_char1(ch1)
{}

MatchResult Detect2Char::match(const QString& text, int offset, const QStringList&)
{
    if (text.size() - offset < 2)
        return offset;
    if (text.at(offset) == m_char && text.at(offset + 1) == m_char1)
        return offset + 2;
    return offset;
}

}

