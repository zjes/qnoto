#include "detectspaces.h"

namespace syntax {

int DetectSpaces::match(const QString& text, int offset)
{
    while(offset < text.size() && text.at(offset).isSpace())
        ++offset;

    return offset;
}

}
