#pragma once
#include "common.h"

namespace syntax {

FORWARD(Context)
FORWARD(Definition)
FORWARD(Rule)

class Context: public Base
{
public:
    Context(const DefinitionPtr& def, const QString& name);
public:
    void setAttribute(const QString& attr);
    void setDynamic(bool dynamic);
    void setFallThrough(bool fall);
    void setFallThroughContext(const QString& cnt);
    void setLineBeginContext(const QString& cnt);
    void setLineEndContext(const QString& cnt);
public:
    const QString& name() const;
    const QString& attribute() const;
    const QList<RulePtr>& rules() const;
    const QString& lineEndContext() const;
    const QString& lineBeginContext() const;
public:
    void addRule(const RulePtr& rule);
    void addRule(const QList<RulePtr>& rules);
    void removeRule(const RulePtr& rule);
private:
    DefinitionPtr  m_definition;
    QString        m_name;
    QString        m_attribute;
    bool           m_dynamic;
    bool           m_fallThrought;
    QString        m_fallThroughtCnt;
    QString        m_lineBeginCnt;
    QString        m_lineEndCnt;
    QList<RulePtr> m_rules;
};

}
