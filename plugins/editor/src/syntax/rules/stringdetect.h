#pragma once
#include "rule.h"

namespace syntax {

FORWARD(StringDetect)

class StringDetect: public Rule
{
public:
    StringDetect(const QString& str, bool insensitive);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QString             m_string;
    Qt::CaseSensitivity m_insensitive;
};

}
