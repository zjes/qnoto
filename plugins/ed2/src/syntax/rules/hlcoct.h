#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCOct)

class HlCOct: public Rule
{
public:
    HlCOct();
    int match(const QString& text, int offset) override;
};

}
