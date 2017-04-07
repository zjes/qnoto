#include "hlcchar.h"

namespace syntax {

MatchResult HlCChar::match(const QString& text, int offset, const QStringList&)
{
    if (text.size() < offset + 3)
        return offset;

    if (text[offset] != '\'')
        return offset;

    int newOff = offset+1;
    if (text[newOff] == '\\')
        ++newOff;

    if (text[newOff+1] != '\'')
        return offset;

    return newOff+2;
}

}
