#include <QRegExp>
#include "hlcoct.h"

namespace syntax {

MatchResult HlCOct::match(const QString& text, int offset, const QStringList&)
{
    static QRegExp rx("0o[0-7]+", Qt::CaseInsensitive);
    int result = rx.indexIn(text, offset);

    if (result == -1)
        return offset;

    return offset + rx.matchedLength();
}

}
