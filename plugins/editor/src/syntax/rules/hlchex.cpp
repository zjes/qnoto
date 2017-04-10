#include <QRegExp>
#include "hlchex.h"

namespace syntax {

int HlCHex::match(const QString& text, int offset)
{
    static QRegExp rx("0x[0-9abcdef]+", Qt::CaseInsensitive);
    int result = rx.indexIn(text, offset);

    if (result  != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
