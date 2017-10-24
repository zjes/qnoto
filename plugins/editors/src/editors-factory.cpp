#include <QDebug>
#include <QFileInfo>
#include "editors-factory.h"
#include "includes/pluginmanager.h"

qnoto::EditorInstance* EditorsFactory::create(const QString& fname)
{
    if (auto* ed = qnoto::plugin<qnoto::Editor>()->create()){
        ed->init(fname);
        return ed;
    }
    return nullptr;
}
