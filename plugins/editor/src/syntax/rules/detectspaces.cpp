#include "detectspaces.h"

namespace syntax {

MatchResult DetectSpaces::match(const QString& text, int offset, const QStringList&)
{
    while(offset < text.size() && text.at(offset).isSpace())
        ++offset;
    return offset;
}

}
