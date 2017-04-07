#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCChar)

class HlCChar: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
