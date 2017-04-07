#include <QRegExp>
#include "hlcstringchar.h"

namespace syntax {

MatchResult HlCStringChar::match(const QString& text, int offset, const QStringList&)
{
    static QRegExp rx("\\[abefnrtv\"\'\\?\\\\.]+", Qt::CaseSensitive);
    int result = rx.indexIn(text, offset);

    if (result == -1)
        return offset;

    return offset + rx.matchedLength();
}

}
