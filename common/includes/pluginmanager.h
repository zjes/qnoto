#pragma once
#include <QString>
#include <QSharedPointer>
#include "plugin.h"

namespace qnoto {

//--------------------------------------------------------------------------------------------------

class COMMON_EXPORT PluginDef: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name    MEMBER name    NOTIFY dummy)
    Q_PROPERTY(QString caption MEMBER caption NOTIFY dummy)
    Q_PROPERTY(Type    type    MEMBER type    NOTIFY dummy)
public:
    enum class Type
    {
        Qml,
        Bin
    };
    Q_ENUM(Type)

public:
    PluginDef() = default;
    PluginDef(const QString& _name, const QString& _caption, Type _type, const QString& _fileName, const QString& _mainQml);

signals:
    void dummy();

public:
    QString name;
    QString caption;
    Type    type;
    QString fileName;
    QString mainQml;
};

using PluginDefPtr = QSharedPointer<PluginDef>;

//--------------------------------------------------------------------------------------------------

class COMMON_EXPORT PluginManager
{
public:
    virtual ~PluginManager();

    static PluginManager& instance();
    bool preloadPlugins();

    template<typename Plug>
    Plug* plugin()
    {
        if (Plugin* plug = plugin(qobject_interface_iid<Plug*>())){
            if (Plug* out = qobject_cast<Plug*>(plug))
                return out;
        }
        return nullptr;
    }

    template<typename Plug>
    QList<Plugin*> plugins() const
    {
        return plugins(qobject_interface_iid<Plug*>());
    }

    QList<Plugin*> plugins(const QString& interfaceName) const;
    Plugin* plugin(const QString& name);
    PluginDef* pluginDefinition(const QString& name) const;
    QList<PluginDef*> pluginDefinitions(const QString& filter) const;

private:
    PluginManager();
private:
    class PluginManagerImpl;
    QScopedPointer<PluginManagerImpl> m_impl;
};

//--------------------------------------------------------------------------------------------------

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

inline Plugin* plugin(const QString& name)
{
    return PluginManager::instance().plugin(name);
}

inline PluginDef* pluginDefinition(const QString& name)
{
    return PluginManager::instance().pluginDefinition(name);
}

inline QList<PluginDef*> pluginDefinitions(const QString& filter)
{
    return PluginManager::instance().pluginDefinitions(filter);
}


//--------------------------------------------------------------------------------------------------


}

Q_DECLARE_METATYPE(qnoto::PluginDef::Type)
