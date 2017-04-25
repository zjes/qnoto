#pragma once
#include <QWidget>
#include <QMap>
#include <QStackedWidget>
#include <QList>
#include "includes/plugins/editors.h"
#include "includes/plugins/editor.h"

class HistoryNavigate;
class EditorsFind;

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
    void showFind() override;
private:
    void historyNavigate();
    void delNavigator(const QString& fileName);
    void escape();
private:
    struct Editor
    {
        qnoto::EditorInstance* editor;
        int                    index;
    };

    QStackedWidget*        m_widget = nullptr;
    HistoryNavigate*       m_historyNavi = nullptr;
    QMap<QString, Editor>  m_editors;
    QList<QString>         m_stack;
    QMenu*                 m_menuEdit = nullptr;
    EditorsFind*           m_find = nullptr;
};
