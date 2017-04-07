#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Float)

class Float: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
