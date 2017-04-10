#pragma once
#include "rule.h"

namespace syntax {

FORWARD(WordDetect)

class WordDetect: public Rule
{
public:
    WordDetect(const QString& str);
    int match(const QString& text, int offset) override;
private:
    QString m_string;
};

}
