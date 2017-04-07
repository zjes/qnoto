#include <QDebug>
#include "keywordlist.h"

namespace syntax {

KeywordList::KeywordList(const QString& name):
    m_name(name)
{}

void KeywordList::add(const QString& value)
{
    m_values.append(value.trimmed());
}

const QStringList& KeywordList::values() const
{
    return m_values;
}

}
