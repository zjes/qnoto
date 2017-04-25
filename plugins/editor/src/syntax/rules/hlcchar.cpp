#include <QRegularExpression>
#include "hlcchar.h"

namespace syntax {

int HlCChar::match(const QString& text, int offset)
{
    static QRegularExpression rx("'[" + QRegularExpression::escape("\\") + "]{0,2}.'", QRegularExpression::CaseInsensitiveOption | QRegularExpression::OptimizeOnFirstUsageOption);
    auto result = rx.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
