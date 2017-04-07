#include <QDebug>
#include "keyword.h"
#include "../keywordlist.h"

namespace syntax {

Keyword::Keyword(const QString& listName):
    m_listName(listName)
{
}

MatchResult Keyword::match(const QString& text, int offset, const QStringList&)
{
    if (!m_list)
        return offset;

    if (offset > 0 && !isDelimeter(text[offset-1]))
        return offset;

    for(const QString& kwd: m_list->values()){
        if (offset + kwd.length() >= text.length())
            continue;
        if (
            (offset + kwd.length() == text.length() || isDelimeter(text[offset + kwd.length()])) &&
            text.midRef(offset, kwd.length()).compare(kwd) == 0
        ){
            return offset+kwd.length();
        }
    }

    return offset;
}

const QString& Keyword::listName() const
{
    return m_listName;
}

void Keyword::setKeywordList(const KeywordListPtr& list)
{
    m_list = list;
}

}
