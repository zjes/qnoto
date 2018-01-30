#include "int.h"

namespace syntax {

Int::Int():
    Rule("Int")
{}

int Int::match(const QString& text, int offset)
{
    while(offset < text.size() && text.at(offset).isDigit())
        ++offset;
    return offset;
}

}
