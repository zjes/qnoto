#pragma once
#include "rule.h"

namespace syntax {

FORWARD(DetectChar)

class DetectChar: public Rule
{
public:
    DetectChar(QChar ch);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QChar m_char;
};

}
