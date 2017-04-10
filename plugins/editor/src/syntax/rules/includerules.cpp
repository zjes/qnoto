#include <QDebug>
#include "includerules.h"

namespace syntax {

IncludeRules::IncludeRules(bool includeAttrib):
    m_includeAttrib(includeAttrib)
{}

int IncludeRules::match(const QString& /*text*/, int offset)
{
    qWarning() << "Include" << context() << "not implemented yet, wrong place here";
    return offset;
}

}
