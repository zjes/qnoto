#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Detect2Char)

class Detect2Char: public Rule
{
public:
    Detect2Char(const QChar& ch, const QChar& ch1);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QChar m_char;
    QChar m_char1;
};

}
