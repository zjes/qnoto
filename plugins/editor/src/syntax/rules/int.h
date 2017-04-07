#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Int)

class Int: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
