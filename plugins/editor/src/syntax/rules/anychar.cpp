#include "anychar.h"

namespace syntax {

AnyChar::AnyChar(const QString& chars):
    m_chars(chars)
{}

MatchResult AnyChar::match(const QString& text, int offset, const QStringList&)
{
    if (m_chars.contains(text.at(offset)))
        return offset + 1;
    return offset;
}

}
