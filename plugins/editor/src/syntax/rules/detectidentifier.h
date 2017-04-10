#pragma once
#include "rule.h"

namespace syntax {

FORWARD(DetectIdentifier)

class DetectIdentifier: public Rule
{
public:
    int match(const QString& text, int offset) override;
};

}
