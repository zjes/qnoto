#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Int)

class Int: public Rule
{
public:
    Int();
    int match(const QString& text, int offset) override;
};

}
