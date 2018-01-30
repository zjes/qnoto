#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCChar)

class HlCChar: public Rule
{
public:
    HlCChar();
    int match(const QString& text, int offset) override;
};

}
