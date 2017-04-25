#include <QDebug>
#include <QFileInfo>
#include "editors-factory.h"
#include "includes/pluginmanager.h"

qnoto::EditorInstance* EditorsFactory::create(const QString& fname)
{
    QFileInfo info(fname);
    if (info.suffix() == "py")
        qDebug() << "python";

    auto* ed = qnoto::plugin<qnoto::Editor>()->create();
    qDebug() << ed;
    if (ed){
        ed->init(fname);
        return ed;
    }
    return nullptr;
}
