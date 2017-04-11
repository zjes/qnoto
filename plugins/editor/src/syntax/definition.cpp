#include <QDebug>
#include "definition.h"
#include "keywordlist.h"
#include "context.h"
#include "rules/includerules.h"
#include "rules/keyword.h"
#include "item-data.h"

namespace syntax {

KeywordListPtr Definition::keywordList(const QString& name)
{
    if (!m_keywords.contains(name))
        m_keywords.insert(name, KeywordListPtr::create(name));
    return m_keywords[name];
}

ContextPtr Definition::createContext(const QString& name)
{
    ContextPtr ctx = ContextPtr::create(cast(Definition, sharedFromThis()), name);
    if (!m_contexts.size())
        m_default = ctx;
    m_contexts.insert(name, ctx);
    return m_contexts.value(name);
}

ContextPtr Definition::context(const QString& name)
{
    if (name.isEmpty())
        return m_default;

    return m_contexts.value(name);
}

void Definition::init()
{
    for(const auto& ctx: m_contexts){
        for(const RulePtr& rule: ctx->rules()){
            if (!cast(Keyword, rule))
                continue;
            auto kwd = cast(Keyword, rule);
            kwd->setKeywordList(keywordList(kwd->listName()));
        }
    }

    for(const auto& ctx: m_contexts){
        bool found = true;
        while(found){
            found = false;
            for(const RulePtr& rule: ctx->rules()){
                if (!cast(IncludeRules, rule))
                    continue;
                ContextPtr fromctx = context(rule->context());
                ctx->removeRule(rule);
                if (fromctx)
                    ctx->addRule(fromctx->rules());
                found = true;
                break;
            }
        }
    }
    m_items.insert("dsWhitespace", ItemDataPtr::create("Whitespace", "dsWhitespace"));
    m_items.insert("dsFoundMark",  ItemDataPtr::create("FoundMark", "dsFoundMark"));
}

void Definition::addItemData(const ItemDataPtr& item)
{
    m_items.insert(item->name(), item);
}

ItemDataPtr Definition::itemData(const QString& name) const
{
    return m_items.value(name);
}


}
