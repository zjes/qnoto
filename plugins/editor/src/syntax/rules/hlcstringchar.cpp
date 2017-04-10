#include <QRegExp>
#include "hlcstringchar.h"

namespace syntax {

int HlCStringChar::match(const QString& text, int offset)
{
    static QRegExp rx("\\[abefnrtv\"\'\\?\\\\.]+", Qt::CaseSensitive);
    int result = rx.indexIn(text, offset);

    if (result != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
