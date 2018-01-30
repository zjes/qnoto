#include "worddetect.h"

namespace syntax {

WordDetect::WordDetect(const QString& str):
    Rule("WordDetect"),
    m_regexp("\\b" + str + "\\b", QRegularExpression::OptimizeOnFirstUsageOption)
{}

int WordDetect::match(const QString& text, int offset)
{
    auto result = m_regexp.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
