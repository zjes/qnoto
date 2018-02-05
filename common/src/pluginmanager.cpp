#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QPluginLoader>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonArray>
#include <QQmlEngine>
#include <QResource>
#include "includes/pluginmanager.h"
#include "includes/plugin.h"

namespace qnoto {

class PluginManagerImpl
{
public:
    bool preloadPlugins()
    {
        QMutexLocker lock(&m_mutex);

        QFile file("plugins.json");
        if (!file.exists()){
            qWarning() << "plugins.json doesn't exists";
            return false;
        }

        if (!file.open(QIODevice::ReadOnly)){
            qWarning() << "plugins.json cannot be read" << file.errorString();
            return false;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error){
            qWarning() << "plugins.json canont be parsed" << err.errorString();
            return false;
        }

        file.close();

        for(const QJsonValue& val: doc.array()){
            QJsonObject obj = val.toObject();

            auto def = PluginDefPtr::create();
            def->name     = obj.value("typeName").toString();
            def->fileName = obj.value("fileName").toString();
            def->mainQml  = obj.value("mainQml").toString();
            def->caption  = obj.value("name").toString();
            def->type     = obj.value("type").toString() == "bin" ? PluginDef::Type::Bin : PluginDef::Type::Qml;

            m_definitions.insert(obj.value("typeName").toString(), def);
        }

        for(const auto& def: m_definitions){
            if (def->type == PluginDef::Type::Bin){
                auto loader = QSharedPointer<QPluginLoader>::create(def->fileName);
                if (!loader->load()){
                    qWarning() << loader->errorString();
                    continue;
                }
                qDebug() << "Preload:" << loader->metaData()["IID"].toString();
                m_plugins.insert(
                    loader->metaData()["IID"].toString(),
                    loader
                );
            } else {
                QResource::registerResource(def->fileName);
            }
        }

        return true;
    }

public:
    QMultiHash<QString, PluginDefPtr> m_definitions;
    QMultiHash<QString, QSharedPointer<QPluginLoader>> m_plugins;
    QMutex m_mutex;
};

//--------------------------------------------------------------------------------------------------

PluginManager& PluginManager::instance()
{
    static PluginManager inst;
    return inst;
}

PluginManager::PluginManager():
    m_impl(new PluginManagerImpl)
{}

PluginManager::~PluginManager()
{
}

bool PluginManager::preloadPlugins()
{
    return m_impl->preloadPlugins();
}

Plugin* PluginManager::plugin(const QString& name) const
{
    for(const QString& key: m_impl->m_plugins.keys()){
        if (!key.startsWith(name))
            continue;

        if (!(name.length() == key.length() || (name.length() < key.length() && key[name.length()] == '.')))
            continue;

        QObject* inst = m_impl->m_plugins.value(key)->instance();
        if (!inst){
            qWarning() << m_impl->m_plugins.value(key)->errorString();
            return nullptr;
        }
        Plugin* out = qobject_cast<Plugin*>(inst);
        if (!out)
            qWarning() << "Cannot cast" << inst << "to Plugin";

        return out;
    }

    qWarning() << "Plugin with name" << name << "was not found";
    return nullptr;
}

QList<Plugin*> PluginManager::plugins(const QString& interfaceName) const
{
    QMutexLocker lock(&m_impl->m_mutex);
    QList<Plugin*> ret;

    for(const QString& key: m_impl->m_plugins.keys()){
        if (!key.startsWith(interfaceName))
            continue;

        if (!(interfaceName.length() == key.length() || (interfaceName.length() < key.length() && key[interfaceName.length()] == '.')))
            continue;

        for(const auto& loader: m_impl->m_plugins.values(key)){
            ret.append(qobject_cast<Plugin*>(loader->instance()));
        }
    }
    return ret;
}

PluginDef* PluginManager::pluginDefinition(const QString& name) const
{
    if (m_impl->m_definitions.contains(name)){
        return m_impl->m_definitions.value(name).data();
    }
    return nullptr;
}

QList<PluginDef*> PluginManager::pluginDefinitions(const QString& filter) const
{
    QList<PluginDef*> ret;
    for(const QString& key: m_impl->m_definitions.keys()){
        if (!filter.isEmpty()){
            if (!key.startsWith(filter))
                continue;

            if (filter.length() < key.length() && !key.startsWith(filter+"."))
                continue;
        }

        ret.append(m_impl->m_definitions.value(key).data());
    }
    return ret;
}

//--------------------------------------------------------------------------------------------------

PluginDef::PluginDef()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

//--------------------------------------------------------------------------------------------------

}
