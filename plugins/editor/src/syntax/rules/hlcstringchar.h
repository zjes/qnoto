#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCStringChar)

class HlCStringChar: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
