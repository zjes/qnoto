#include "context.h"

namespace syntax {

Context::Context(const DefinitionPtr& def, const QString& name):
    m_definition(def),
    m_name(name)
{}

void Context::setAttribute(const QString& attr)
{
    m_attribute = attr;
}

void Context::setDynamic(bool dynamic)
{
    m_dynamic = dynamic;
}

void Context::setFallThrough(bool fall)
{
    m_fallThrought = fall;
}

void Context::setFallThroughContext(const QString& cnt)
{
    m_fallThroughtCnt = cnt;
}

void Context::setLineBeginContext(const QString& cnt)
{
    m_lineBeginCnt = cnt;
}

void Context::setLineEndContext(const QString& cnt)
{
    m_lineEndCnt = cnt;
}

void Context::addRule(const RulePtr& rule)
{
    m_rules.append(rule);
}

const QString& Context::name() const
{
    return m_name;
}

const QString& Context::attribute() const
{
    return m_attribute;
}

const QList<RulePtr>& Context::rules() const
{
    return m_rules;
}

const QString& Context::lineEndContext() const
{
    return m_lineEndCnt;
}

const QString& Context::lineBeginContext() const
{
    return m_lineBeginCnt;
}

void Context::addRule(const QList<RulePtr>& rules)
{
    m_rules.append(rules);
}

void Context::removeRule(const RulePtr& rule)
{
    m_rules.removeAll(rule);
}

}
