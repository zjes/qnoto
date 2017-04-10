#include "detectidentifier.h"

namespace syntax {

int DetectIdentifier::match(const QString& text, int offset)
{
    static QRegExp rx("[a-z0-9_]+", Qt::CaseInsensitive);
    int result = rx.indexIn(text, offset);

    if (result != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
