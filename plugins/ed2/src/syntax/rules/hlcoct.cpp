#include <QRegularExpression>
#include "hlcoct.h"

namespace syntax {

HlCOct::HlCOct():
    Rule("HlCOct")
{}

int HlCOct::match(const QString& text, int offset)
{
    static QRegularExpression rx("0o[0-7]+", QRegularExpression::CaseInsensitiveOption | QRegularExpression::OptimizeOnFirstUsageOption);
    auto result = rx.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
