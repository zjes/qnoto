#include "int.h"

namespace syntax {

MatchResult Int::match(const QString& text, int offset, const QStringList&)
{
    while(offset < text.size() && text.at(offset).isDigit())
        ++offset;
    return offset;
}

}
