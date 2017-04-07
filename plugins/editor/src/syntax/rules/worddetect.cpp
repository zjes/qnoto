#include <QRegExp>
#include "worddetect.h"

namespace syntax {

WordDetect::WordDetect(const QString& str):
    m_string(str)
{}

MatchResult WordDetect::match(const QString& text, int offset, const QStringList&)
{
    static QRegExp rx("\\b"+m_string+"\\b");
    int result = rx.indexIn(text, offset);

    if (result == -1)
        return offset;

    return offset + rx.matchedLength();
}

}
