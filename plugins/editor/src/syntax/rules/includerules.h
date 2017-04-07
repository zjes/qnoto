#pragma once
#include "rule.h"

namespace syntax {

FORWARD(IncludeRules)

class IncludeRules: public Rule
{
public:
    IncludeRules(bool includeAttrib);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    bool m_includeAttrib;
};

}
