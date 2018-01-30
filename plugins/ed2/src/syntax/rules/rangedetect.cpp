#include "rangedetect.h"

namespace syntax {

RangeDetect::RangeDetect(const QChar& ch, const QChar& ch1):
    Rule("RangeDetect"),
    m_char(ch),
    m_char1(ch1)
{}

int RangeDetect::match(const QString& text, int offset)
{
    if (text.size() - offset < 2 || text.at(offset) != m_char)
        return offset;

    auto newOffset = offset + 1;
    while (newOffset < text.size()) {
        if (text.at(newOffset) == m_char1)
            return newOffset + 1;
        ++newOffset;
    }
    return offset;
}

}
