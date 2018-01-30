#pragma once
#include "common.h"

namespace syntax {

FORWARD(KeywordList)

class KeywordList: public Base
{
public:
    KeywordList(const QString& name);
    void add(const QString& value);
    const QStringList& values() const;
private:
    QString     m_name;
    QStringList m_values;
};

}
