#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCHex)

class HlCHex: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
