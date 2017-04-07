#pragma once
#include <QWidget>
#include <QMap>
#include "includes/plugins/leftside.h"

class QStackedWidget;
class QComboBox;

class Header : public QWidget
{
    Q_OBJECT
public:
    Header(const QList<qnoto::LeftSidePtr>& plugins, const QString& selected, QWidget* parent);
    void addItem(qnoto::LeftSide* plugin);
    QString selectedPluginName() const;

signals:
    void addSplit(const QString& name);
    void closeSplit();
    void showPlugin(const QString & name);
private:
    QComboBox *m_box;
};

class LeftSideContainter : public QWidget
{
    Q_OBJECT
public:
    explicit LeftSideContainter(const QString& selected, QWidget *parent = 0);
    QString selectedPlugin() const;
signals:
    void addSplit(const QString& name);
    void closeSplit();
private:
    void showPlugin(const QString & name);
    void collectPlugins();
private:
    QStackedWidget* m_stack;
    QMap<QString, int> m_stackIndex;
    QList<qnoto::LeftSidePtr> m_plugins;
    Header* m_header;
};
