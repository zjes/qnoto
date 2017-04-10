#pragma once
#include "rule.h"

namespace syntax {

FORWARD(DetectChar)

class DetectChar: public Rule
{
public:
    DetectChar(QChar ch);
    int match(const QString& text, int offset) override;
private:
    QChar m_char;
};

}
