#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCOct)

class HlCOct: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
