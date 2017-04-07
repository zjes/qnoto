#pragma once
#include "common.h"

namespace syntax {

FORWARD(Definition)
FORWARD(KeywordList)
FORWARD(Context)
FORWARD(ItemData)

class Definition: public Base
{
public:
    void init();
public:
    KeywordListPtr keywordList(const QString& name);
    ContextPtr createContext(const QString& name);
    ContextPtr context(const QString& name = {});
    void addItemData(const ItemDataPtr& item);
    ItemDataPtr itemData(const QString& name) const;
private:
    QHash<QString, KeywordListPtr> m_keywords;
    QHash<QString, ContextPtr>     m_contexts;
    ContextPtr                     m_default;
    QHash<QString, ItemDataPtr>    m_items;
};

}
