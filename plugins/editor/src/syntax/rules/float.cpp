#include <QDebug>
#include <QRegExp>
#include "float.h"

namespace syntax {

int Float::match(const QString& text, int offset)
{
    static QRegExp rx("((\\d+\\.\\d*)|(\\.\\d+))([eE]\\-\\d+)?f?");
    int result = rx.indexIn(text, offset);

    if (result != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
