#pragma once
#include <QWidget>
#include <QMap>

#include "includes/plugins/tool-plugin.h"

class QStackedWidget;
class QComboBox;

class Header : public QWidget
{
    Q_OBJECT
public:
    Header(const QList<qnoto::ToolPlugin*>& plugins, const QString& selected, QWidget* parent);
    void addItem(qnoto::ToolPlugin* plugin);
    QString selectedPluginName() const;

signals:
    void addSplit(const QString& name);
    void closeSplit();
    void showPlugin(const QString & name);
private:
    QComboBox *m_box;
};

class ToolPlace : public QWidget
{
    Q_OBJECT
public:
    explicit ToolPlace(const QString& selected, QWidget *parent = 0);
    QString selectedPlugin() const;
    void restoreState();
signals:
    void addSplit(const QString& name);
    void closeSplit();
private:
    void showPlugin(const QString & name);
    void collectPlugins();
private:
    QStackedWidget* m_stack;
    QMap<QString, int> m_stackIndex;
    QList<qnoto::ToolPlugin*> m_plugins;
    Header* m_header;
    QString m_selected;
};
