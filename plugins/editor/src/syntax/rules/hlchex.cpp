#include <QRegExp>
#include "hlchex.h"

namespace syntax {

MatchResult HlCHex::match(const QString& text, int offset, const QStringList&)
{
    static QRegExp rx("0x[0-9abcdef]+", Qt::CaseInsensitive);
    int result = rx.indexIn(text, offset);

    if (result == -1)
        return offset;

    return offset + rx.matchedLength();
}

}
