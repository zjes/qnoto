#include "linecontinue.h"

namespace syntax {

LineContinue::LineContinue(const QChar& ch):
    m_char(ch)
{}

int LineContinue::match(const QString& text, int offset)
{
    if (offset == text.size() - 1 && text.at(offset) == m_char)
        return offset + 1;

    return offset;
}

}
