#pragma once
#include "common.h"

class QDomElement;

namespace syntax {

FORWARD(Definition)

class Parser
{
public:
    Parser(const QString& fname);
    void parse(DefinitionPtr& def);
private:
    QString m_fname;
};

}
