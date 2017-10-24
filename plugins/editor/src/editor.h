#pragma once
#include <QPlainTextEdit>
#include "includes/plugins/editor.h"

class EditorImpl;
class EditorHeader;

class EditorInstance: public qnoto::EditorInstance
{
    Q_OBJECT
public:
    EditorInstance();
    ~EditorInstance() override;
    bool init(const QString& fileName) override;
    bool isModified() const override;
    const QString& fileName() const override;
    bool save(const QString& newFileName = {}) override;
    const QList<QAction*>& actions() override;

    void setSearch(const QString& text = {}) override;
    QString selectedText() const override;
    void doSearch() override;

protected:
    void focusInEvent(QFocusEvent *) override;
private:
    void escape();
private:
    QString                      m_fileName;
    QScopedPointer<EditorImpl>   m_widget;
    QScopedPointer<EditorHeader> m_header;
    QList<QAction*>              m_actions;
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
