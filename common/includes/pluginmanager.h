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
    PluginDef();

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

public:
    QList<Plugin*>    plugins(const QString& interfaceName) const;
    Plugin*           plugin(const QString& name) const;
    PluginDef*        pluginDefinition(const QString& name) const;
    QList<PluginDef*> pluginDefinitions(const QString& filter) const;

private:
    PluginManager();

private:
    QScopedPointer<class PluginManagerImpl> m_impl;
};

//--------------------------------------------------------------------------------------------------

inline Plugin* plugin(const QString& name)
{ return PluginManager::instance().plugin(name); }

inline PluginDef* pluginDefinition(const QString& name)
{ return PluginManager::instance().pluginDefinition(name); }

inline QList<PluginDef*> pluginDefinitions(const QString& filter)
{ return PluginManager::instance().pluginDefinitions(filter); }

//--------------------------------------------------------------------------------------------------

}

Q_DECLARE_METATYPE(qnoto::PluginDef::Type)
