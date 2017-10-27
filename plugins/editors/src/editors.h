#pragma once
#include <QWidget>
#include <QMap>
#include <QStackedWidget>
#include <QList>
#include <QMutex>
#include "includes/plugins/editors.h"
#include "includes/plugins/editor.h"
#include "editors-state.h"
#include "editors-actions.h"

class EditorsFind;

struct EditorInfo
{
    qnoto::EditorInstance* editor;
    int                    index;
};

class Editors: public qnoto::Editors
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.Editors")
    Q_INTERFACES(qnoto::Editors)

public:
    Editors();
    ~Editors() override;

public:
    void openFile(const QString& file) override;
    void closeFile(const QString& fileName) override;
    const MenuList& actions() const override;

public:
    QString name() const override;
    QString title() const override;
    bool saveState() const override;
    void restoreState() override;

public:
    qnoto::EditorInstance* currentEditor() const;
    const QMap<QString, EditorInfo>& editors() const;

    template<typename... T>
    MenuList actions(const T&... names) const
    { return m_actions.actions(names...); }

signals:
    void openedChanged();

private:
    void escape();
    void showFind();

private:
    QMutex                          m_mutex;
    QStackedWidget*                 m_widget = nullptr;
    QMap<QString, EditorInfo>       m_editors;
    EditorsFind*                    m_find = nullptr;
    QString                         m_current;
    EditorsState                    m_state;
    EditorsActions                  m_actions;
};
