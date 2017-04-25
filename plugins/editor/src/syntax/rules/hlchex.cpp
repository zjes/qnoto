#include <QRegularExpression>
#include "hlchex.h"

namespace syntax {

int HlCHex::match(const QString& text, int offset)
{
    static QRegularExpression rx("0x[0-9abcdef]+", QRegularExpression::CaseInsensitiveOption | QRegularExpression::OptimizeOnFirstUsageOption);
    auto result = rx.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
