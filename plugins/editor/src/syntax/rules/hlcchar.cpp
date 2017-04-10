#include "hlcchar.h"

namespace syntax {

int HlCChar::match(const QString& text, int offset)
{
    static QRegExp rx("'["+QRegExp::escape("\\")+"]{0,2}.'");

    int result = rx.indexIn(text, offset);

    if (result != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
