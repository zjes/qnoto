#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Float)

class Float: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
