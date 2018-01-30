#pragma once
#include <QAbstractListModel>
#include "settings.h"

namespace qnoto
{ class PluginDef; }

class PluginsModel: public QAbstractListModel
{
    Q_OBJECT
public:
    PluginsModel(QObject* parent = nullptr);
    PluginsModel(const QList<qnoto::PluginDef*>& data);
    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    qnoto::PluginDef* value(int index);
    int length();
private:
    QList<qnoto::PluginDef*> m_data;
};

Q_DECLARE_METATYPE(PluginsModel*)
