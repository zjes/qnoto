#pragma once
#include <QString>
#include <QSharedPointer>
#include "plugin.h"

namespace qnoto {

class COMMON_EXPORT PluginManager
{
public:
    virtual ~PluginManager();

    static PluginManager& instance();
    bool preloadPlugins();

    template<typename Plug>
    Plug* plugin()
    {
        Plugin* p = plugin(qobject_interface_iid<Plug*>());
        if (p){
            Plug* out = qobject_cast<Plug*>(p);
            if (!out)
                return nullptr;
            return out;
        }
        return nullptr;
    }

    Plugin* plugin(const QString& name);

    template<typename Plug>
    QList<Plugin*> plugins() const
    { return plugins(qobject_interface_iid<Plug*>()); }

    QList<Plugin*> plugins(const QString& interfaceName) const;
private:
    PluginManager();
private:
    class PluginManagerImpl;
    QScopedPointer<PluginManagerImpl> m_impl;
};

template<typename Plug>
inline Plug* plugin()
{
    return PluginManager::instance().plugin<Plug>();
}

template<typename Plug>
inline QList<Plugin*> plugins()
{
    return PluginManager::instance().plugins<Plug>();
}

inline QList<Plugin*> plugins(const QString& prefix)
{
    return PluginManager::instance().plugins(prefix);
}

}
