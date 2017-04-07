#pragma once
#include "rule.h"

namespace syntax {

FORWARD(Keyword)
FORWARD(KeywordList)

class Keyword: public Rule
{
public:
    Keyword(const QString& listName);
    MatchResult match(const QString& text, int offset, const QStringList&) override;
    const QString& listName() const;
    void setKeywordList(const KeywordListPtr& list);
private:
    QString m_listName;
    KeywordListPtr m_list;
};

}
