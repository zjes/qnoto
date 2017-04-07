#include <QDebug>
#include "includerules.h"

namespace syntax {

IncludeRules::IncludeRules(bool includeAttrib):
    m_includeAttrib(includeAttrib)
{}

MatchResult IncludeRules::match(const QString& /*text*/, int offset, const QStringList&)
{
    qWarning() << "Include" << context() << "not implemented yet, wrong place here";
    return offset;
}

}
