#pragma once
#include "rule.h"

namespace syntax {

FORWARD(RegExpr)

class RegExpr: public Rule
{
public:
    RegExpr(const QString& str, bool insensitive, bool minimal);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QRegExp m_regexp;
};

}
