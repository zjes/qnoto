#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QPluginLoader>
#include <QMutex>
#include "includes/pluginmanager.h"
#include "includes/plugin.h"

namespace qnoto {

class PluginManager::PluginManagerImpl: public QObject
{
    Q_OBJECT
public:
    ~PluginManagerImpl() override
    {}

    bool preloadPlugins()
    {
        QMutexLocker lock(&m_mutex);
        QDir pluginsDir(QCoreApplication::applicationDirPath());
        pluginsDir.cd("plugins");

        for(const QFileInfo& inf: pluginsDir.entryInfoList({"*.so", "*.dll"})) {
            auto loader = QSharedPointer<QPluginLoader>::create(inf.absoluteFilePath());
            if (!loader->load()){
                qWarning() << loader->errorString();
                continue;
            }
            qDebug() << "Preload:" << loader->metaData()["IID"].toString();
            m_plugins.insert(
                loader->metaData()["IID"].toString(),
                loader
            );
        }

        return false;
    }

public:
    QMultiHash<QString, QSharedPointer<QPluginLoader>> m_plugins;
    QMutex m_mutex;
};

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

Plugin* PluginManager::plugin(const QString& name)
{
    //QMutexLocker lock(&m_impl->m_mutex);

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

}

#include "pluginmanager.moc"
