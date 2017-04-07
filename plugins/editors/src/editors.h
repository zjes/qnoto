#pragma once
#include <QWidget>
#include <QMap>
#include <QStackedWidget>
#include <QList>
#include "includes/plugins/editors.h"
#include "includes/plugins/editor.h"

class HistoryNavigate;

class Editors: public qnoto::Editors
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.Editors")
    Q_INTERFACES(qnoto::Editors)
public:
    Editors();
    ~Editors();
    void openFile(const QString& file) override;
    void saveState(QSettings& sets) const override;
    void restoreState(QSettings& sets) override;
    QString name() const override;
    QString title() const override;
    void setMenu(QMenu* menu) override;
private:
    void historyNavigate();
    void delNavigator(const QString& fileName);
private:
    QStackedWidget* m_widget;
    HistoryNavigate* m_historyNavi;
    struct Editor
    {
        qnoto::EditorInstance* editor;
        int                    index;
    };
    QMap<QString, Editor> m_editors;
    QList<QString> m_stack;
    QMenu* m_menuEdit;
};
