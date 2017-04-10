#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCHex)

class HlCHex: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
