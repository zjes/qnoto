#pragma once
#include <QString>
#include <QList>
#include "common.h"

class QFileInfo;

namespace syntax {

FORWARD(Definition)

class Manager
{
public:
    static Manager& instance();
    DefinitionPtr definition(const QString& file);
private:
    Manager();
    void readFile(const QFileInfo& info);
    void initDefinition(DefinitionPtr& def, const QString& file);

private:
    struct Info
    {
        QString       name;
        QString       pattern;
        QString       syntaxFile;
        DefinitionPtr definition;
    };
    QList<Info> m_info;
};

}
