#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Float)

class Float: public Rule
{
public:
    Float();
    int match(const QString& text, int offset) override;
};

}
