#include <QDebug>
#include <QRegularExpression>
#include "float.h"

namespace syntax {

Float::Float():
    Rule("Float")
{}

int Float::match(const QString& text, int offset)
{
    static QRegularExpression rx("((\\d+\\.\\d*)|(\\.\\d+))([eE]\\-\\d+)?f?", QRegularExpression::CaseInsensitiveOption | QRegularExpression::OptimizeOnFirstUsageOption);
    auto result = rx.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
