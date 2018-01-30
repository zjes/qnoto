#pragma once
#include "rule.h"

namespace syntax {

FORWARD(AnyChar)

class AnyChar: public Rule
{
public:
    AnyChar(const QString& chars);
    int match(const QString& text, int offset) override;
private:
    QString m_chars;
};

}
