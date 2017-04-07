#pragma once
#include "rule.h"

namespace syntax {

FORWARD(RangeDetect)

class RangeDetect: public Rule
{
public:
    RangeDetect(const QChar& ch, const QChar& ch1);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QChar m_char;
    QChar m_char1;
};

}
