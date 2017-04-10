#pragma once
#include "rule.h"

namespace syntax {

FORWARD(IncludeRules)

class IncludeRules: public Rule
{
public:
    IncludeRules(bool includeAttrib);
    int match(const QString& text, int offset) override;
private:
    bool m_includeAttrib;
};

}
