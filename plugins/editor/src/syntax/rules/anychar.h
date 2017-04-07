#pragma once
#include "rule.h"

namespace syntax {

FORWARD(AnyChar)

class AnyChar: public Rule
{
public:
    AnyChar(const QString& chars);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QString m_chars;
};

}
