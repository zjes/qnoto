#pragma once
#include "rule.h"

namespace syntax {

FORWARD(DetectSpaces)

class DetectSpaces: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
