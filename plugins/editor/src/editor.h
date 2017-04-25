#pragma once
#include <QPlainTextEdit>
#include "includes/plugins/editor.h"

class EditorImpl;
class EditorHeader;

class EditorInstance: public qnoto::EditorInstance
{
    Q_OBJECT
public:
    ~EditorInstance();
    bool init(const QString& fileName) override;
    void populateMenu(qnoto::Editors* manager, QMenu* menu) override;

    void setSearch(const QString& text = {}) override;
    QString selectedText() const override;
    void doSearch() override;

protected:
    void focusInEvent(QFocusEvent *) override;
private:
    void escape();
private:
    QScopedPointer<EditorImpl>   m_widget;
    QScopedPointer<EditorHeader> m_header;
    QScopedPointer<QMenu>        m_editMenu;
};

class Editor: public qnoto::Editor
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.Editor.DefaultEditor")
    Q_INTERFACES(qnoto::Editor)
public:
    qnoto::EditorInstance* create() override;
    QString name() const override;
    QString title() const override;
    qnoto::PreferencesPage* preferences() const override;
};
