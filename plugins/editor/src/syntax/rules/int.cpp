#include "int.h"

namespace syntax {

int Int::match(const QString& text, int offset)
{
    while(offset < text.size() && text.at(offset).isDigit())
        ++offset;
    return offset;
}

}
