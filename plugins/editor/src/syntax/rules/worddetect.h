#pragma once
#include "rule.h"

namespace syntax {

FORWARD(WordDetect)

class WordDetect: public Rule
{
public:
    WordDetect(const QString& str);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
private:
    QString m_string;
};

}
