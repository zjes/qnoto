#pragma once
#include <QPlainTextEdit>
#include "includes/plugins/editor.h"

class EditorImpl;
class EditorHeader;
class EditorFind;

class EditorInstance: public qnoto::EditorInstance
{
    Q_OBJECT
public:
    ~EditorInstance();
    bool init(const QString& fileName) override;
    void populateMenu(QMenu* menu) override;

protected:
    void focusInEvent(QFocusEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    QScopedPointer<EditorImpl>   m_widget;
    QScopedPointer<EditorHeader> m_header;
    QScopedPointer<EditorFind>   m_find;
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
