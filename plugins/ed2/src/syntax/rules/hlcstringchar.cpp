#include <QRegularExpression>
#include "hlcstringchar.h"

namespace syntax {

HlCStringChar::HlCStringChar():
    Rule("HlCStringChar")
{}

int HlCStringChar::match(const QString& text, int offset)
{
    static QRegularExpression rx("\\[abefnrtv\"\'\\?\\\\.]+", QRegularExpression::OptimizeOnFirstUsageOption);
    auto result = rx.match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (!result.hasMatch())
        return offset;

    return offset + result.capturedLength(0);
}

}
