#include <QDebug>
#include "regexpr.h"

namespace syntax {

RegExpr::RegExpr(const QString& str, bool insensitive, bool minimal):
    Rule("RegExpr"),
    m_regexp(str, QRegularExpression::OptimizeOnFirstUsageOption),
    m_string(str)
{
    auto options = m_regexp.patternOptions();
    if (!insensitive)
        options |= QRegularExpression::CaseInsensitiveOption;
    if (minimal)
        options |= QRegularExpression::InvertedGreedinessOption;

    m_regexp.setPatternOptions(options);

    if (!m_regexp.isValid()){
        qWarning() << "Regexp ! valid" << m_regexp.errorString();
    }
}

int RegExpr::match(const QString& text, int offset)
{
    auto result = m_regexp.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
