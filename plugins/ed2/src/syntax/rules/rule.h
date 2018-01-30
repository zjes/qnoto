#pragma once
#include "../common.h"

namespace syntax {

FORWARD(Rule)

class Rule: public Base
{
public:
    Rule(const QString& name);
    void addRule(const RulePtr& rule);
public:
    void setAttribute(const QString& attr);
    void setFirstNonSpace(bool fns);
    void setLookAhead(bool look);
    void setColumn(int col);
    void setDynamic(bool dyn);
    void setBeginRegion(const QString& name);
    void setEndRegion(const QString& name);
    void setContext(const QString& cnt);
public:
    const QString& context() const;
    const QString& attribute() const;
    const QString& beginRegion() const;
    const QString& endRegion() const;
    bool lookAhead() const;
public:
    virtual int match(const QString& text, int offset) = 0;
protected:
    QString        m_name;
    QList<RulePtr> m_rules;
    QString        m_attribute;
    bool           m_firstNonSpace = false;
    bool           m_lookAhead = false;
    int            m_column = 0;
    bool           m_dynamic  = false;
    QString        m_beginRegion;
    QString        m_endRegion;
    QString        m_context;
};

}
