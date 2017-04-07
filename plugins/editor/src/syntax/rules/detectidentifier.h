#pragma once
#include "rule.h"

namespace syntax {

FORWARD(DetectIdentifier)

class DetectIdentifier: public Rule
{
public:
    MatchResult match(const QString& text, int offset, const QStringList&) override;
};

}
