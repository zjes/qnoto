#pragma once
#include "../common.h"

namespace syntax {

FORWARD(Rule)

struct MatchResult
{
    MatchResult(int off):
        offset(off)
    {}

    MatchResult(int off, const QStringList& capt):
        offset(off),
        captures(capt)
    {}

    MatchResult(int off, int skip):
        offset(off),
        skipOffset(skip)
    {}

    int         offset = 0;
    int         skipOffset = 0;
    QStringList captures = {};
};

class Rule: public Base
{
public:
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
public:
    virtual MatchResult match(const QString& text, int offset, const QStringList& = {}) = 0;
protected:
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