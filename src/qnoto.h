#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QQuickItem>
#include "settings.h"

//--------------------------------------------------------------------------------------------------

namespace qnoto
{ class PluginDef; }

class PluginsModel;

class QNoto : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Settings*     settings READ settings     NOTIFY dummy)
    Q_PROPERTY(PluginsModel* plugins  READ pluginsModel NOTIFY dummy)
public:
    explicit QNoto(QObject *parent = nullptr);

public slots:
    QObject* loadPlugin(QObject* parent, const QString& name);
    qnoto::PluginDef* plugin(const QString& name);
    PluginsModel* pluginsModel(const QString& filter = "") const;

private:
    Settings*     settings();
signals:
    void dummy();
private:
    Settings m_sets;
    mutable QMap<QString, QSharedPointer<PluginsModel>> m_models;
};
