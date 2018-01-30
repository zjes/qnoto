#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCHex)

class HlCHex: public Rule
{
public:
    HlCHex();
    int match(const QString& text, int offset) override;
};

}
