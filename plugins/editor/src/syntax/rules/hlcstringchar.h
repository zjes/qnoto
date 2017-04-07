#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCStringChar)

class HlCStringChar: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
