#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Int)

class Int: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
