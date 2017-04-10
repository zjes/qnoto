#include <QRegExp>
#include "worddetect.h"

namespace syntax {

WordDetect::WordDetect(const QString& str):
    m_string(str)
{}

int WordDetect::match(const QString& text, int offset)
{
    static QRegExp rx("\\b"+m_string+"\\b");
    int result = rx.indexIn(text, offset);

    if (result != offset)
        return offset;

    return offset + rx.matchedLength();
}

}
