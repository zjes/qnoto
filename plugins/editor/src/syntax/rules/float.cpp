#include <QDebug>
#include <QRegExp>
#include "float.h"

namespace syntax {

MatchResult Float::match(const QString& text, int offset, const QStringList&)
{
    static QRegExp rx("((\\d+\\.\\d*)|(\\.\\d+))([eE]\\-\\d+)?f?");
    int result = rx.indexIn(text, offset);

    if (result == -1)
        return offset;

    return offset + rx.matchedLength();
}

}
