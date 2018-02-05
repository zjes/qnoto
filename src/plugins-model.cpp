#include <QDebug>
#include <QQmlEngine>
#include "plugins-model.h"
#include "includes/pluginmanager.h"
#include "qnoto.h"

PluginsModel::PluginsModel(QObject* parent):
    QAbstractListModel(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PluginsModel::PluginsModel(const QList<qnoto::PluginDef*>& data):
    m_data(data)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

int PluginsModel::rowCount(const QModelIndex&) const
{
    return m_data.length();
}

QVariant PluginsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    auto& it = m_data.at(index.row());
    switch (role) {
    case Qt::UserRole + 2:
        return it->name;
    case Qt::UserRole + 3:
        return it->caption;
    case Qt::UserRole + 4:
        return QVariant::fromValue<qnoto::PluginDef::Type>(it->type);
    }
    return {};
}

QHash<int, QByteArray> PluginsModel::roleNames() const
{
    return {
        { Qt::UserRole+2, "name" },
        { Qt::UserRole+3, "caption" },
        { Qt::UserRole+4, "type" },
    };
}

qnoto::PluginDef* PluginsModel::value(int index)
{
    return m_data[index];
}

int PluginsModel::length()
{
    return m_data.length();
}
