#include <QRegularExpression>
#include "detectidentifier.h"

namespace syntax {

int DetectIdentifier::match(const QString& text, int offset)
{
    static QRegularExpression rx("[a-z0-9_]+", QRegularExpression::CaseInsensitiveOption | QRegularExpression::OptimizeOnFirstUsageOption);
    auto result = rx.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
