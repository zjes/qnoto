#pragma once
#include "rule.h"

namespace syntax {

FORWARD(LineContinue)

class LineContinue: public Rule
{
public:
    LineContinue(const QChar& ch);
    int match(const QString& text, int offset) override;
private:
    QChar m_char;
};

}
