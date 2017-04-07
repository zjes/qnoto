#pragma once
#include "rule.h"

namespace syntax {

FORWARD(LineContinue)

class LineContinue: public Rule
{
public:
    LineContinue(const QChar& ch);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QChar m_char;
};

}
