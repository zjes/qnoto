#include <QDebug>
#include "rule.h"

namespace syntax {

void Rule::addRule(const RulePtr& rule)
{
    m_rules.append(rule);
}

void Rule::setAttribute(const QString& attr)
{
    m_attribute = attr;
}

void Rule::setFirstNonSpace(bool fns)
{
    m_firstNonSpace = fns;
}

void Rule::setLookAhead(bool look)
{
    m_lookAhead = look;
}

void Rule::setColumn(int col)
{
    m_column = col;
}

void Rule::setDynamic(bool dyn)
{
    m_dynamic = dyn;
}

void Rule::setBeginRegion(const QString& name)
{
    m_beginRegion = name;
}

void Rule::setEndRegion(const QString& name)
{
    m_endRegion = name;
}

void Rule::setContext(const QString& cnt)
{
    m_context = cnt;
}

const QString& Rule::context() const
{
    return m_context;
}

const QString& Rule::attribute() const
{
    return m_attribute;
}

}
