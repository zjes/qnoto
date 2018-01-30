#pragma once
#include <QRegularExpression>
#include "rule.h"

namespace syntax {

FORWARD(RegExpr)

class RegExpr: public Rule
{
public:
    RegExpr(const QString& str, bool insensitive, bool minimal);
    int match(const QString& text, int offset) override;
private:
    QRegularExpression m_regexp;
    QString m_string;
};

}
