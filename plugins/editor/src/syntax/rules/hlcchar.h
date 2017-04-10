#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCChar)

class HlCChar: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
