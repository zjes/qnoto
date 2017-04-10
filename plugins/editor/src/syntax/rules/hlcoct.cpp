#include <QRegExp>
#include "hlcoct.h"

namespace syntax {

int HlCOct::match(const QString& text, int offset)
{
    static QRegExp rx("0o[0-7]+", Qt::CaseInsensitive);
    int result = rx.indexIn(text, offset);

    if (result != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
