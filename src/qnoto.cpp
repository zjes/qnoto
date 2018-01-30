#include <QDebug>
#include <QQmlEngine>

#include "qnoto.h"
#include "includes/pluginmanager.h"
#include "includes/file-handler.h"
#include "plugins-model.h"
#include "selectable.h"

//--------------------------------------------------------------------------------------------------

QNoto::QNoto(QObject *parent):
    QObject(parent)
{
    qmlRegisterType<Settings>("qnoto.settings", 1, 0, "Settings");
    qRegisterMetaType<const qnoto::PluginDef*>("const Plugin*");
    qmlRegisterType<qnoto::PluginDef>("qnoto.settings", 1, 0, "Plugin");
    qmlRegisterType<PluginsModel>("qnoto.settings", 1, 0, "Plugins");
    qmlRegisterType<Selectable>("Selectable", 1, 0, "Selectable");
}

QObject* QNoto::loadPlugin(QObject* parent, const QString& name)
{
    auto* pluginDef = qnoto::pluginDefinition(name);
    if (!pluginDef)
        return nullptr;

    if (pluginDef->type == qnoto::PluginDef::Type::Bin){
        qDebug() << "load bin" << name;
        auto* plugin = qnoto::plugin(name);
        if (plugin)
            return plugin->create(parent);
    } else {
        qDebug() << "load qml" << name;
        return qnoto::Plugin::createComponent(parent, pluginDef->mainQml);
    }

    return nullptr;
}

Settings* QNoto::settings()
{
    return &m_sets;
}

PluginsModel* QNoto::pluginsModel(const QString& filter) const
{
    if (!m_models.contains(filter)){
        m_models.insert(filter, QSharedPointer<PluginsModel>::create(qnoto::pluginDefinitions(filter)));
    }
    return m_models[filter].data();
}

qnoto::PluginDef* QNoto::plugin(const QString& name)
{
    return qnoto::pluginDefinition(name);
}
