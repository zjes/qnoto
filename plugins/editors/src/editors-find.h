#pragma once

#include <QTimer>
#include <QWidget>
#include "includes/plugins/editor.h"

class EditorImpl;

namespace Ui
{ class EditorFind; }

class EditorsFind : public QWidget
{
    Q_OBJECT
public:
    EditorsFind();
    ~EditorsFind();
public:
    void activate();
    void setEditor(qnoto::EditorInstance* editor);
protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    void textChanged(const QString& text);
    void doSearch();
private:
    qnoto::EditorInstance* m_editor = nullptr;
    QScopedPointer<Ui::EditorFind> m_ui;
    QTimer m_searchTimer;
};
