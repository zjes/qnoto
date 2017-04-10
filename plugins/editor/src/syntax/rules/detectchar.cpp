#include "detectchar.h"

namespace syntax {

DetectChar::DetectChar(QChar ch):
    m_char(ch)
{}

int DetectChar::match(const QString& text, int offset)
{
    if (text.at(offset) == m_char)
        return offset + 1;

    return offset;
}

}
