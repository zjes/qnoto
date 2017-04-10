#include "anychar.h"

namespace syntax {

AnyChar::AnyChar(const QString& chars):
    m_chars(chars)
{}

int AnyChar::match(const QString& text, int offset)
{
    if (m_chars.contains(text.at(offset)))
        return offset + 1;
    return offset;
}

}
