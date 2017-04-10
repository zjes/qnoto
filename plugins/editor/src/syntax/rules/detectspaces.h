#pragma once
#include "rule.h"

namespace syntax {

FORWARD(DetectSpaces)

class DetectSpaces: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
